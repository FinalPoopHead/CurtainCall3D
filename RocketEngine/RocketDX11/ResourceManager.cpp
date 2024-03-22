#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <cassert>

#include "ResourceManager.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "Mesh.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "GraphicsMacro.h"
#include "texture.h"
#include "material.h"
#include "VertexStruct.h"

#include "CubeMap.h"

const std::string TEXTURE_PATH = "Resources/Textures/";
const std::string MODEL_PATH = "Resources/Models/";

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

		// Color Shader
		{
			VertexShader* colorVS = new VertexShader();
			colorVS->Initialize(_device.Get(), L"RocketDX11/ColorVS.hlsl");
			colorVS->SetVertexType(eVertexType::COLOR_VERTEX);
			_vertexShaders["ColorVS"] = colorVS;

			PixelShader* colorPS = new PixelShader();
			colorPS->Initialize(_device.Get(), L"RocketDX11/ColorPS.hlsl");
			_pixelShaders["ColorPS"] = colorPS;
		}

		// Texture Shader
		{
			VertexShader* textureVS = new VertexShader();
			textureVS->Initialize(_device.Get(), L"RocketDX11/TextureVS.hlsl");
			textureVS->SetVertexType(eVertexType::TEXTURE_VERTEX);
			_vertexShaders["TextureVS"] = textureVS;

			PixelShader* texturePS = new PixelShader();
			texturePS->Initialize(_device.Get(), L"RocketDX11/TexturePS.hlsl");
			_pixelShaders["TexturePS"] = texturePS;
		}

		// Light Shader
		{
			VertexShader* lightVS = new VertexShader();
			lightVS->Initialize(_device.Get(), L"RocketDX11/LightVS.hlsl");
			lightVS->SetVertexType(eVertexType::LIGHT_VERTEX);
			_vertexShaders["LightVS"] = lightVS;

			PixelShader* lightPS = new PixelShader();
			lightPS->Initialize(_device.Get(), L"RocketDX11/LightPS.hlsl");
			_pixelShaders["LightPS"] = lightPS;
		}

		// StaticMesh Shader
		{
			VertexShader* staticMeshVS = new VertexShader();
			staticMeshVS->Initialize(_device.Get(), L"RocketDX11/StaticMeshVS.hlsl");
			staticMeshVS->SetVertexType(eVertexType::VERTEX);
			_vertexShaders["StaticMeshVS"] = staticMeshVS;

			PixelShader* staticMeshPS = new PixelShader();
			staticMeshPS->Initialize(_device.Get(), L"RocketDX11/StaticMeshPS.hlsl");
			_pixelShaders["StaticMeshPS"] = staticMeshPS;
		}

		// SkinnedMesh Shader
		{
			VertexShader* skinnedMeshVS = new VertexShader();
			skinnedMeshVS->Initialize(_device.Get(), L"RocketDX11/SkinnedMeshVS.hlsl");
			skinnedMeshVS->SetVertexType(eVertexType::SKINNED_VERTEX);
			_vertexShaders["SkinnedMeshVS"] = skinnedMeshVS;

			PixelShader* skinnedMeshPS = new PixelShader();
			skinnedMeshPS->Initialize(_device.Get(), L"RocketDX11/SkinnedMeshPS.hlsl");
			_pixelShaders["SkinnedMeshPS"] = skinnedMeshPS;
		}

		// CubeMap Shader
		{
			VertexShader* cubeMapVS = new VertexShader();
			cubeMapVS->Initialize(_device.Get(), L"RocketDX11/CubeMapVS.hlsl");
			cubeMapVS->SetVertexType(eVertexType::VERTEX);
			_vertexShaders["CubeMapVS"] = cubeMapVS;

			PixelShader* cubeMapPS = new PixelShader();
			cubeMapPS->Initialize(_device.Get(), L"RocketDX11/CubeMapPS.hlsl");
			_pixelShaders["CubeMapPS"] = cubeMapPS;
		}

		CreateRenderStates();

		_cubeMesh = new CubeMesh();
		_cubeMesh->Initialize(device);

		_sphereMesh = new SphereMesh();
		_sphereMesh->Initialize(device);

		_cubeMap = new CubeMap();
		_cubeMap->Initialize(device);
		_cubeMap->LoadTexture("CloudCubeMap.dds");

		_defaultTexture = LoadTextureFile("darkbrickdxt1.dds");

		_defaultFont = new DirectX::SpriteFont(_device.Get(), L"Resources/Font/NotoSansKR.spritefont");
		
		_defaultMaterial = new Material();
		_defaultMaterial->SetVertexShader(GetVertexShader("StaticMeshVS"));
		_defaultMaterial->SetPixelShader(GetPixelShader("StaticMeshPS"));
		_defaultMaterial->SetRenderState(GetRenderState(eRenderState::SOLID));
		_defaultMaterial->SetTexture(_defaultTexture);

		_cubePrimitive = DirectX::DX11::GeometricPrimitive::CreateCube(deviceContext, 1.0f, false);
		_spherePrimitive = DirectX::DX11::GeometricPrimitive::CreateSphere(deviceContext, 1.0f, 8, false, false);
		_cylinderPrimitive = DirectX::DX11::GeometricPrimitive::CreateCylinder(deviceContext, 2.0f, 1.0f, 8, false);
	}

	void ResourceManager::LoadModel(const std::string& fileName, const RawModel* rawModel)
	{
		if (rawModel->animations.empty())
		{
			_models.insert({ fileName,new StaticModel });
			StaticModel* staticModel = ProcessStaticModel(fileName, rawModel);
		}
		else
		{
			_models.insert({ fileName, new DynamicModel });
			DynamicModel* dynamicModel = ProcessDynamicModel(fileName, rawModel);
		}
	}

	VertexShader* ResourceManager::GetVertexShader(const std::string& name)
	{
		if (_vertexShaders.find(name) == _vertexShaders.end())
		{
			return nullptr;
		}

		return _vertexShaders[name];
	}

	PixelShader* ResourceManager::GetPixelShader(const std::string& name)
	{
		if (_pixelShaders.find(name) == _pixelShaders.end())
		{
			return nullptr;
		}

		return _pixelShaders[name];
	}

	DirectX::SpriteFont* ResourceManager::GetDefaultFont()
	{
		return _defaultFont;
	}

	ID3D11Device* ResourceManager::GetDevice()
	{
		return _device.Get();
	}

	ID3D11DeviceContext* ResourceManager::GetDeviceContext()
	{
		return _deviceContext.Get();
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

		D3D11_RASTERIZER_DESC wireframeDesc;
		ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc.CullMode = D3D11_CULL_BACK;
		wireframeDesc.FrontCounterClockwise = false;
		wireframeDesc.DepthClipEnable = true;
		ID3D11RasterizerState* wireframe;
		HR(_device->CreateRasterizerState(&wireframeDesc, &wireframe));
		_renderStates.emplace_back(wireframe);
	}

	ID3D11RasterizerState* ResourceManager::GetRenderState(eRenderState eState)
	{
		return _renderStates[static_cast<int>(eState)];
	}

	Mesh* ResourceManager::GetMesh(eMeshType meshType) const
	{
		switch (meshType)
		{
		case eMeshType::CUBE:
			return _cubeMesh;
		case eMeshType::SPHERE:
			return _sphereMesh;
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

		return _meshes.at(fileName);
	}

	Texture* ResourceManager::GetTexture(std::string fileName)
	{
		if (_textures.find(fileName) == _textures.end())
		{
			return LoadTextureFile(fileName);
		}

		return _textures.at(fileName);
	}

	Texture* ResourceManager::LoadTextureFile(std::string fileName)
	{
		std::string fullPath = TEXTURE_PATH + fileName;
		std::wstring wFileName(fullPath.begin(), fullPath.end());

		std::string extension = fileName.substr(fileName.find_last_of(".") + 1);

		ID3D11Resource* rawTexture = nullptr;
		ID3D11ShaderResourceView* textureView = nullptr;

		if (extension == "dds")
		{
			HR(DirectX::CreateDDSTextureFromFile(_device.Get(), wFileName.c_str(), &rawTexture, &textureView));
		}
		else if (extension == "jpg" || extension == "png")
		{
			HR(DirectX::CreateWICTextureFromFile(_device.Get(), wFileName.c_str(), &rawTexture, &textureView));
		}
		else
		{
			assert(false);
		}

		Texture* texture = new Texture(rawTexture, textureView);

		_textures.insert({ fileName,texture });

		return texture;
	}

	Model* ResourceManager::GetModel(const std::string& fileName)
	{
		if (_models.find(fileName) == _models.end())
		{
			return nullptr;
		}

		return _models[fileName];
	}

	StaticModel* ResourceManager::ProcessStaticModel(const std::string& fileName, const RawModel* rawModel)
	{
		StaticModel* resultModel = reinterpret_cast<StaticModel*>(_models.at(fileName));

		// RawNode정보 순회하면서 Node Hierarchy 만들기. Bone정보도 같이 처리함.
		resultModel->rootNode = ProcessRawNodeRecur(rawModel->rootNode);

		// Mesh 정보 순회하면서 Mesh 만들기. Texture도 이때 로드해봄.
 		for (auto& rawMesh : rawModel->meshes)
		{
			StaticMesh* staticMesh = ProcessStaticMesh(rawMesh);
			resultModel->meshes.push_back(staticMesh);
			_meshes.insert({ rawMesh->name, staticMesh });
		}

		return resultModel;
	}

	DynamicModel* ResourceManager::ProcessDynamicModel(const std::string& fileName, const RawModel* rawModel)
	{
		DynamicModel* resultModel = reinterpret_cast<DynamicModel*>(_models.at(fileName));

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
			vertex.nodeIndex = rawVertex.nodeIndex;

			vertices.push_back(vertex);
		}

		// TODO : 이거 공통코드인데 한번에 처리할 수 있지 않을까?
		if (rawMesh->material)
		{
			for (auto& rawTexture : rawMesh->material->textures)
			{
				if (_textures.find(rawTexture->path) == _textures.end())
				{
					LoadTextureFile(rawTexture->path);
				}
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
				if (_textures.find(rawTexture->path) == _textures.end())
				{
					LoadTextureFile(rawTexture->path);
				}
			}
		}

		SkinnedMesh* skinnedMesh = new SkinnedMesh(vertices, rawMesh->indices);
		skinnedMesh->CreateBuffers();

		return skinnedMesh;
	}

}
