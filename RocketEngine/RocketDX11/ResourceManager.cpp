﻿#include <cassert>
#include <filesystem>

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

	void ResourceManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float screenWidth, float screenHeight)
	{
		_device = device;
		_deviceContext = deviceContext;

		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		std::filesystem::path hlslPath(HLSL_PATH);
		std::filesystem::directory_iterator itr(hlslPath);
		while (itr != std::filesystem::end(itr))
		{
			const std::filesystem::directory_entry& entry = *itr;
			std::string extension = entry.path().extension().string();
			if (extension == ".hlsl")
			{
				std::string pureName = entry.path().filename().string();
				pureName = pureName.substr(0, pureName.find_last_of("."));

				if (pureName.find("VS") != std::string::npos)
				{
					std::unique_ptr<VertexShader> vs = std::make_unique<VertexShader>();
					vs->Initialize(_device, HLSL_PATH + entry.path().filename().wstring());
					_vertexShaders[pureName] = std::move(vs);
				}
				else if (pureName.find("PS") != std::string::npos)
				{
					std::unique_ptr<PixelShader> ps = std::make_unique<PixelShader>();
					ps->Initialize(_device, HLSL_PATH + entry.path().filename().wstring());
					_pixelShaders[pureName] = std::move(ps);
				}
			}
			itr++;
		}

		CreateRenderStates();

		_cubeMesh = std::make_unique<CubeMesh>();
		_cubeMesh->Initialize(device);

		_sphereMesh = std::make_unique<SphereMesh>();
		_sphereMesh->Initialize(device);

		_defaultTexture = GetTexture("darkbrickdxt1.dds");
		//_defaultTexture = GetTexture("CubeTexture.png");

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
		if (_models.find(fileName) != _models.end())
		{
			return;
		}

		if (rawModel->animations.empty())
		{
			_models.insert({ fileName, std::make_unique<StaticModel>() });
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
		//shadowMapDesc.CullMode = D3D11_CULL_NONE;		// TODO : Plane의 경우를 어떻게 처리할것인가 (그림자를 만들거면 NONE, 아니라면 BACK)
		shadowMapDesc.CullMode = D3D11_CULL_BACK;
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
		if (_meshes.find(fileName) == _meshes.end())
		{
			return nullptr;
		}

		return _meshes.at(fileName).get();
	}

	Rocket::Core::Texture* ResourceManager::GetTexture(std::string filePath, bool isFullPath /*= true*/)
	{
		std::string fullPath = filePath;

		if(!isFullPath)
		{
			/// 경로 제외하기 위한 로직
			size_t slashIndex = filePath.find_last_of("/\\");
			if (slashIndex != std::string::npos)
			{
				fullPath = filePath.substr(slashIndex + 1, filePath.length() - slashIndex);
			}
			else
			{
				fullPath = filePath;
			}
		}

		if (_textures.find(fullPath) == _textures.end())
		{
			std::unique_ptr<Texture> texture = std::make_unique<Texture>();
			texture->LoadFromFile(_device, fullPath, isFullPath);
			_textures.insert({ fullPath,std::move(texture) });
		}

		return _textures.at(fullPath).get();
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
			_meshes.insert({ rawMesh->name + std::to_string(meshNameCount[rawMesh->name]), std::move(staticMesh) });	// TODO : 근데 다른 노드인데 이름이 같은 경우면 어떡하지? 세상에~
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
