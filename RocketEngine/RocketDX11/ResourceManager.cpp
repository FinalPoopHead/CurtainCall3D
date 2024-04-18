#include <cassert>

#include "ResourcePath.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "GraphicsMacro.h"
#include "VertexStruct.h"
#include "CubeMap.h"

namespace Rocket::Core
{
	template <typename T>
	ULONG GetRefCount(const ComPtr<T>& p)
	{
		T* temp = p.Get();

		ULONG ret = 0;
		if (temp != nullptr)
		{
			ret = temp->AddRef();
			ret = temp->Release();
		}

		return ret;
	}
}

namespace Rocket::Core
{
	ResourceManager::ResourceManager()
		: _device(),
		_deviceContext(),
		_defaultFont(),
		_cubeMesh(),
		_sphereMesh(),
		_defaultTexture(),
		_defaultMaterial()
	{

	}

	void ResourceManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		_device = device;
		_deviceContext = deviceContext;

		// TODO : 이거 자동화 해서 해당 폴더에 있는 모든 hlsl 이름에 맞게 저장할 수 있도록 하는게 좋을 듯?
		// Color Shader
		{
			std::unique_ptr<VertexShader> colorVS = std::make_unique<VertexShader>();
			colorVS->Initialize(_device, HLSL_PATH + L"ColorVS.hlsl");
			colorVS->SetVertexType(eVertexType::COLOR_VERTEX);
			_vertexShaders["ColorVS"] = std::move(colorVS);

			std::unique_ptr<PixelShader> colorPS = std::make_unique<PixelShader>();
			colorPS->Initialize(_device, HLSL_PATH + L"ColorPS.hlsl");
			_pixelShaders["ColorPS"] = std::move(colorPS);
		}

		// Texture Shader
		{
			std::unique_ptr<VertexShader> textureVS = std::make_unique<VertexShader>();
			textureVS->Initialize(_device, HLSL_PATH + L"TextureVS.hlsl");
			textureVS->SetVertexType(eVertexType::TEXTURE_VERTEX);
			_vertexShaders["TextureVS"] = std::move(textureVS);

			std::unique_ptr<PixelShader> texturePS = std::make_unique<PixelShader>();
			texturePS->Initialize(_device, HLSL_PATH + L"TexturePS.hlsl");
			_pixelShaders["TexturePS"] = std::move(texturePS);
		}

		// Light Shader
		{
			std::unique_ptr<VertexShader> lightVS = std::make_unique<VertexShader>();
			lightVS->Initialize(_device, HLSL_PATH + L"LightVS.hlsl");
			lightVS->SetVertexType(eVertexType::LIGHT_VERTEX);
			_vertexShaders["LightVS"] = std::move(lightVS);

			std::unique_ptr<PixelShader> lightPS = std::make_unique<PixelShader>();
			lightPS->Initialize(_device, HLSL_PATH + L"LightPS.hlsl");
			_pixelShaders["LightPS"] = std::move(lightPS);
		}

		// StaticMesh Shader
		{
			std::unique_ptr<VertexShader> staticMeshVS = std::make_unique<VertexShader>();
			staticMeshVS->Initialize(_device, HLSL_PATH + L"StaticMeshVS.hlsl");
			staticMeshVS->SetVertexType(eVertexType::VERTEX);
			_vertexShaders["StaticMeshVS"] = std::move(staticMeshVS);

			std::unique_ptr<PixelShader> staticMeshPS = std::make_unique<PixelShader>();
			staticMeshPS->Initialize(_device, HLSL_PATH + L"StaticMeshPS.hlsl");
			_pixelShaders["StaticMeshPS"] = std::move(staticMeshPS);
		}

		// SkinnedMesh Shader
		{
			std::unique_ptr<VertexShader> skinnedMeshVS = std::make_unique<VertexShader>();
			skinnedMeshVS->Initialize(_device, HLSL_PATH + L"SkinnedMeshVS.hlsl");
			skinnedMeshVS->SetVertexType(eVertexType::SKINNED_VERTEX);
			_vertexShaders["SkinnedMeshVS"] = std::move(skinnedMeshVS);

			std::unique_ptr<PixelShader> skinnedMeshPS = std::make_unique<PixelShader>();
			skinnedMeshPS->Initialize(_device, HLSL_PATH + L"SkinnedMeshPS.hlsl");
			_pixelShaders["SkinnedMeshPS"] = std::move(skinnedMeshPS);
		}

		// CubeMap Shader
		{
			std::unique_ptr<VertexShader> cubeMapVS = std::make_unique<VertexShader>();
			cubeMapVS->Initialize(_device, HLSL_PATH + L"CubeMapVS.hlsl");
			cubeMapVS->SetVertexType(eVertexType::VERTEX);
			_vertexShaders["CubeMapVS"] = std::move(cubeMapVS);

			std::unique_ptr<PixelShader> cubeMapPS = std::make_unique<PixelShader>();
			cubeMapPS->Initialize(_device, HLSL_PATH + L"CubeMapPS.hlsl");
			_pixelShaders["CubeMapPS"] = std::move(cubeMapPS);
		}

		// Deferred StaticMesh Shader
		{
			std::unique_ptr<VertexShader> deferredStaticMeshVS = std::make_unique<VertexShader>();
			deferredStaticMeshVS->Initialize(_device, HLSL_PATH + L"DeferredStaticMeshVS.hlsl");
			deferredStaticMeshVS->SetVertexType(eVertexType::VERTEX);
			_vertexShaders["DeferredStaticMeshVS"] = std::move(deferredStaticMeshVS);

			std::unique_ptr<PixelShader> deferredStaticMeshPS = std::make_unique<PixelShader>();
			deferredStaticMeshPS->Initialize(_device, HLSL_PATH + L"DeferredStaticMeshPS.hlsl");
			_pixelShaders["DeferredStaticMeshPS"] = std::move(deferredStaticMeshPS);
		}

		// Deferred SkinnedMesh Shader
		{
			std::unique_ptr<VertexShader> deferredSkinnedMeshVS = std::make_unique<VertexShader>();
			deferredSkinnedMeshVS->Initialize(_device, HLSL_PATH + L"DeferredSkinnedMeshVS.hlsl");
			deferredSkinnedMeshVS->SetVertexType(eVertexType::SKINNED_VERTEX);
			_vertexShaders["DeferredSkinnedMeshVS"] = std::move(deferredSkinnedMeshVS);

			std::unique_ptr<PixelShader> deferredSkinnedMeshPS = std::make_unique<PixelShader>();
			deferredSkinnedMeshPS->Initialize(_device, HLSL_PATH + L"DeferredSkinnedMeshPS.hlsl");
			_pixelShaders["DeferredSkinnedMeshPS"] = std::move(deferredSkinnedMeshPS);
		}

		// LightPass Shader
		{
			std::unique_ptr<VertexShader> lightPassVS = std::make_unique<VertexShader>();
			lightPassVS->Initialize(_device, HLSL_PATH + L"LightPassVS.hlsl");
			lightPassVS->SetVertexType(eVertexType::VERTEX);
			_vertexShaders["LightPassVS"] = std::move(lightPassVS);

			std::unique_ptr<PixelShader> lightPassPS = std::make_unique<PixelShader>();
			lightPassPS->Initialize(_device, HLSL_PATH + L"LightPassPS.hlsl");
			_pixelShaders["LightPassPS"] = std::move(lightPassPS);
		}

		// Deferred Color Shader
		{
			std::unique_ptr<VertexShader> DeferredColorVS = std::make_unique<VertexShader>();
			DeferredColorVS->Initialize(_device, HLSL_PATH + L"DeferredColorVS.hlsl");
			DeferredColorVS->SetVertexType(eVertexType::COLOR_VERTEX);
			_vertexShaders["DeferredColorVS"] = std::move(DeferredColorVS);

			std::unique_ptr<PixelShader> DeferredColorPS = std::make_unique<PixelShader>();
			DeferredColorPS->Initialize(_device, HLSL_PATH + L"DeferredColorPS.hlsl");
			_pixelShaders["DeferredColorPS"] = std::move(DeferredColorPS);
		}

		// Deferred CubeMap Shader
		{
			std::unique_ptr<VertexShader> DeferredCubeMapVS = std::make_unique<VertexShader>();
			DeferredCubeMapVS->Initialize(_device, HLSL_PATH + L"DeferredCubeMapVS.hlsl");
			DeferredCubeMapVS->SetVertexType(eVertexType::VERTEX);
			_vertexShaders["DeferredCubeMapVS"] = std::move(DeferredCubeMapVS);

			std::unique_ptr<PixelShader> DeferredCubeMapPS = std::make_unique<PixelShader>();
			DeferredCubeMapPS->Initialize(_device, HLSL_PATH + L"DeferredCubeMapPS.hlsl");
			_pixelShaders["DeferredCubeMapPS"] = std::move(DeferredCubeMapPS);
		}

		CreateRenderStates();

		_cubeMesh = std::make_unique<CubeMesh>();
		_cubeMesh->Initialize(device);

		_sphereMesh = std::make_unique<SphereMesh>();
		_sphereMesh->Initialize(device);

		_defaultTexture = GetTexture("darkbrickdxt1.dds");

		_defaultFont = std::make_unique<DirectX::SpriteFont>(_device, (FONT_PATH + L"NotoSansKR.spritefont").c_str());
		
		_defaultMaterial = std::make_unique<Material>();
		_defaultMaterial->SetVertexShader(GetVertexShader("StaticMeshVS"));
		_defaultMaterial->SetPixelShader(GetPixelShader("StaticMeshPS"));
		_defaultMaterial->SetRenderState(GetRenderState(eRenderState::SOLID));
		_defaultMaterial->SetBaseColorTexture(_defaultTexture);

		_cubePrimitive = DirectX::DX11::GeometricPrimitive::CreateCube(deviceContext, 1.0f, false);
		_spherePrimitive = DirectX::DX11::GeometricPrimitive::CreateSphere(deviceContext, 1.0f, 8, false, false);
		_cylinderPrimitive = DirectX::DX11::GeometricPrimitive::CreateCylinder(deviceContext, 2.0f, 1.0f, 8, false);
	}

	void ResourceManager::LoadModel(const std::string& fileName, const RawModel* rawModel)
	{
		if (rawModel->animations.empty())
		{
			_models.insert({ fileName,std::make_unique<StaticModel>() });
			ProcessStaticModel(fileName, rawModel);
		}
		else
		{
			_models.insert({ fileName, std::make_unique<DynamicModel>() });
			ProcessDynamicModel(fileName, rawModel);
		}
	}

	VertexShader* ResourceManager::GetVertexShader(const std::string& name)
	{
		if (_vertexShaders.find(name) == _vertexShaders.end())
		{
			return nullptr;
		}

		return _vertexShaders[name].get();
	}

	PixelShader* ResourceManager::GetPixelShader(const std::string& name)
	{
		if (_pixelShaders.find(name) == _pixelShaders.end())
		{
			return nullptr;
		}

		return _pixelShaders[name].get();
	}

	DirectX::SpriteFont* ResourceManager::GetDefaultFont() const
{
		return _defaultFont.get();
	}

	ID3D11Device* ResourceManager::GetDevice()
	{
		return _device;
	}

	ID3D11DeviceContext* ResourceManager::GetDeviceContext()
	{
		return _deviceContext;
	}

	void ResourceManager::CreateRenderStates()
	{
		// Render State 중 Rasterizer State
		D3D11_RASTERIZER_DESC solidDesc;
		ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
		solidDesc.FillMode = D3D11_FILL_SOLID;
		solidDesc.CullMode = D3D11_CULL_BACK;
		solidDesc.FrontCounterClockwise = false;
		solidDesc.DepthClipEnable = true;
		ID3D11RasterizerState* solid;
		HR(_device->CreateRasterizerState(&solidDesc, &solid));
		_renderStates.emplace_back(solid);
		solid->Release();
		solid->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"SOLID RASTER") - 1, L"SOLID RASTER");
		

		D3D11_RASTERIZER_DESC wireframeDesc;
		ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc.CullMode = D3D11_CULL_BACK;
		wireframeDesc.FrontCounterClockwise = false;
		wireframeDesc.DepthClipEnable = true;
		ID3D11RasterizerState* wireframe;
		HR(_device->CreateRasterizerState(&wireframeDesc, &wireframe));
		_renderStates.emplace_back(wireframe);
		wireframe->Release();
		wireframe->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"WIREFRAME RASTER") - 1, L"WIREFRAME RASTER");

		D3D11_RASTERIZER_DESC shadowMapDesc;
		ZeroMemory(&shadowMapDesc, sizeof(D3D11_RASTERIZER_DESC));
		shadowMapDesc.FillMode = D3D11_FILL_SOLID;
		shadowMapDesc.CullMode = D3D11_CULL_FRONT;
		shadowMapDesc.FrontCounterClockwise = false;
		shadowMapDesc.DepthClipEnable = true;
		ID3D11RasterizerState* shadowMapRS;
		HR(_device->CreateRasterizerState(&shadowMapDesc, &shadowMapRS));
		_renderStates.emplace_back(shadowMapRS);
		shadowMapRS->Release();

		D3D11_RASTERIZER_DESC cubeMapDesc;
		ZeroMemory(&cubeMapDesc, sizeof(D3D11_RASTERIZER_DESC));
		cubeMapDesc.FillMode = D3D11_FILL_SOLID;
		cubeMapDesc.CullMode = D3D11_CULL_NONE;
		cubeMapDesc.FrontCounterClockwise = false;
		cubeMapDesc.DepthClipEnable = true;
		ID3D11RasterizerState* cubemapRS;
		HR(_device->CreateRasterizerState(&cubeMapDesc, &cubemapRS));
		_renderStates.emplace_back(cubemapRS);
		cubemapRS->Release();
	}

	ID3D11RasterizerState* ResourceManager::GetRenderState(eRenderState eState)
	{
		return _renderStates[static_cast<int>(eState)].Get();
	}

	Mesh* ResourceManager::GetMesh(eMeshType meshType) const
	{
		switch (meshType)
		{
		case eMeshType::CUBE:
			return _cubeMesh.get();
		case eMeshType::SPHERE:
			return _sphereMesh.get();
		default:
			return nullptr;
		}
	}

	Mesh* ResourceManager::GetMesh(const std::string& fileName)
	{
		if(_meshes.find(fileName) == _meshes.end())
		{
			return nullptr;
		}

		return _meshes.at(fileName).get();
	}

	Texture* ResourceManager::GetTexture(std::string fileName)
	{
		if (_textures.find(fileName) == _textures.end())
		{
			std::unique_ptr<Texture> texture = std::make_unique<Texture>();
			texture->LoadFromFile(_device, fileName);
			_textures.insert({ fileName,std::move(texture) });
		}

		return _textures.at(fileName).get();
	}

	Model* ResourceManager::GetModel(const std::string& fileName)
	{
		if (_models.find(fileName) == _models.end())
		{
			return nullptr;
		}

		return _models[fileName].get();
	}

	StaticModel* ResourceManager::ProcessStaticModel(const std::string& fileName, const RawModel* rawModel)
	{
		StaticModel* resultModel = reinterpret_cast<StaticModel*>(_models.at(fileName).get());

		// RawNode정보 순회하면서 Node Hierarchy 만들기. Bone정보도 같이 처리함.
		resultModel->rootNode = ProcessRawNodeRecur(rawModel->rootNode);

		// 이름 겹치는 경우 대비
		std::unordered_map<std::string, int> meshNameCount;

		// Mesh 정보 순회하면서 Mesh 만들기. Texture도 이때 로드해봄.
 		for (auto& rawMesh : rawModel->meshes)
		{
			std::unique_ptr<StaticMesh> staticMesh(ProcessStaticMesh(rawMesh));
			resultModel->meshes.push_back(staticMesh.get());
			_meshes.insert({ rawMesh->name + std::to_string(meshNameCount[rawMesh->name]), std::move(staticMesh)});	// TODO : 근데 다른 노드인데 이름이 같은 경우면 어떡하지? 세상에~
			meshNameCount[rawMesh->name]++;
		}

		return resultModel;
	}

	DynamicModel* ResourceManager::ProcessDynamicModel(const std::string& fileName, const RawModel* rawModel)
	{
		DynamicModel* resultModel = reinterpret_cast<DynamicModel*>(_models.at(fileName).get());

		// RawNode정보 순회하면서 Node Hierarchy 만들기. Bone정보도 같이 처리함.
		resultModel->rootNode = ProcessRawNodeRecur(rawModel->rootNode);

		// Mesh 정보 순회하면서 Mesh 만들기. Texture도 이때 로드해봄.
		for (auto& rawMesh : rawModel->meshes)
		{
			SkinnedMesh* skinnedMesh = ProcessSkinnedMesh(rawMesh);
			resultModel->meshes.push_back(skinnedMesh);
		}

		// Animation 정보 순회하면서 Animation 만들기
		for (auto& iter : rawModel->animations)
		{
			RawAnimation* rawAnimation = iter.second;
			Animation* animation = new Animation();
			animation->name = rawAnimation->name;
			animation->duration = rawAnimation->duration;
			animation->ticksPerSecond = rawAnimation->ticksPerSecond;

			for (auto& nodeAnim : rawAnimation->nodeAnimations)
			{
				NodeAnimationData* myNodeAnim = new NodeAnimationData();
				myNodeAnim->nodeName = nodeAnim->nodeName;
				myNodeAnim->positionTimestamps = nodeAnim->positionTimestamps;
				myNodeAnim->rotationTimestamps = nodeAnim->rotationTimestamps;
				myNodeAnim->scaleTimestamps = nodeAnim->scaleTimestamps;
				myNodeAnim->positions = nodeAnim->positions;
				myNodeAnim->rotations = nodeAnim->rotations;
				myNodeAnim->scales = nodeAnim->scales;

				animation->nodeAnimations.push_back(myNodeAnim);
			}

			resultModel->animations.insert({ animation->name,animation });
		}

		return resultModel;
	}

	Node* ResourceManager::ProcessRawNodeRecur(const RawNode* rawNode)
	{
		Node* node = new Node();
		node->name = rawNode->name;
		node->index = rawNode->index;
		int test1 = node->index;
		node->transformMatrix = rawNode->transformMatrix;

		RawBone* rawBone = rawNode->bindedBone;
		if (rawBone)
		{
			Bone* bone = new Bone();
			bone->name = rawBone->name;
			bone->index = rawBone->index;
			int test2 = bone->index;
			bone->offsetMatrix = rawBone->offsetMatrix;
			bone->bindedNode = node;

			node->bindedBone = bone;
		}

		for (auto& child : rawNode->children)
		{
			Node* childNode = ProcessRawNodeRecur(child);
			node->children.push_back(childNode);
			childNode->parent = node;
		}

		return node;
	}

	StaticMesh* ResourceManager::ProcessStaticMesh(const RawMesh* rawMesh)
	{
		std::vector<Vertex> vertices;
		for (auto& rawVertex : rawMesh->vertices)
		{
			Vertex vertex;

			vertex.position = rawVertex.position;
			vertex.normal = rawVertex.normal;
			vertex.UV = rawVertex.UV;
			vertex.tangent = rawVertex.tangent;
			vertex.bitangent = rawVertex.biTangent;
			vertex.nodeIndex = rawVertex.nodeIndex;

			vertices.push_back(vertex);
		}

		// TODO : 이거 공통코드인데 한번에 처리할 수 있지 않을까?
		if (rawMesh->material)
		{
			for (auto& rawTexture : rawMesh->material->textures)
			{
				GetTexture(rawTexture->path);
			}
		}

		StaticMesh* staticMesh = new StaticMesh(vertices, rawMesh->indices);
		staticMesh->CreateBuffers();

		return staticMesh;
	}

	SkinnedMesh* ResourceManager::ProcessSkinnedMesh(const RawMesh* rawMesh)
	{
		std::vector<VertexSkinned> vertices;
		for (auto& rawVertex : rawMesh->vertices)
		{
			VertexSkinned vertex;

			vertex.position = rawVertex.position;
			vertex.UV = rawVertex.UV;
			vertex.normal = rawVertex.normal;
			vertex.tangent = rawVertex.tangent;
			vertex.bitangent = rawVertex.biTangent;
			vertex.nodeIndex = rawVertex.nodeIndex;
			vertex.weights = rawVertex.weights;
			vertex.boneIndices = rawVertex.boneIndices;

			vertices.push_back(vertex);
		}

		// TODO : 이거 공통코드인데 한번에 처리할 수 있지 않을까?
		if (rawMesh->material)
		{
			for (auto& rawTexture : rawMesh->material->textures)
			{
				GetTexture(rawTexture->path);
			}
		}

		SkinnedMesh* skinnedMesh = new SkinnedMesh(vertices, rawMesh->indices);
		skinnedMesh->CreateBuffers();

		return skinnedMesh;
	}

	void ResourceManager::Finalize()
	{
		_cubeMesh.reset();
		_sphereMesh.reset();
		_defaultMaterial.reset();
		_defaultTexture = nullptr;
		_defaultFont.reset();
		_cubePrimitive.reset();
		_spherePrimitive.reset();
		_cylinderPrimitive.reset();

		for (auto& iter : _vertexShaders)
		{
			iter.second.reset();
		}

		for (auto& iter : _pixelShaders)
		{
			iter.second.reset();
		}

		for (auto& iter : _textures)
		{
			iter.second.reset();
		}

		delete _defaultTexture;

		for (auto& iter : _models)
		{
			DeleteNodeRecur(iter.second->rootNode);

			auto temp = dynamic_cast<DynamicModel*>(iter.second.get());
			if (temp)
			{
				for (auto& anim : temp->animations)
				{
					for (auto& nodeAnim : anim.second->nodeAnimations)
					{
						delete nodeAnim;
					}
					delete anim.second;
				}

				for (auto& mesh : iter.second.get()->GetMeshes())
				{
					delete mesh;
				}
			}

			iter.second.reset();
		}

		for (auto& rs : _renderStates)
		{
			rs.Reset();
		}

		for (auto& iter : _meshes)
		{
			iter.second.reset();
		}
	}

	void ResourceManager::DeleteNodeRecur(Node* node)
	{
		for (auto& child : node->children)
		{
			DeleteNodeRecur(child);
		}

		if (node->bindedBone)
		{
			delete node->bindedBone;
		}

		delete node;
	}
}
