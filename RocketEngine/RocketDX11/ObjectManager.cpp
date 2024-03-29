#include "ObjectManager.h"

#include "..\\RocketCommon\\IRenderable.h"

#include "ResourceManager.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "DynamicModelRenderer.h"
#include "SpriteRenderer.h"
#include "LineRenderer.h"
#include "Mesh.h"
#include "CubeMesh.h"
#include "DirectionalLight.h"

namespace Rocket::Core
{
	ObjectManager::ObjectManager()
		: _resourceManager(ResourceManager::Instance()),
		_lineRenderer(nullptr),
		_staticModelRendererList(),
		_cameraList(),
		_textList()
	{
	}

	void ObjectManager::Initialize(ID3D11Device* device)
	{
		auto& _rscMgr = ResourceManager::Instance();

		_fpsText.reset(new TextRenderer());
		_fpsText->SetFont(_rscMgr.GetDefaultFont());

		_axis.reset(new Axis());
		_axis->Initialize(device);
		_axis->SetRenderState(_rscMgr.GetRenderState(ResourceManager::eRenderState::WIREFRAME));
		_axis->SetShader(_rscMgr.GetVertexShader("ColorVS"), _rscMgr.GetPixelShader("ColorPS"));

		_grid.reset(new Grid());
		_grid->Initialize(device);
		_grid->SetRenderState(_rscMgr.GetRenderState(ResourceManager::eRenderState::WIREFRAME));
		_grid->SetShader(_rscMgr.GetVertexShader("ColorVS"), _rscMgr.GetPixelShader("ColorPS"));

		auto cubeMap = std::make_unique<CubeMap>();
		cubeMap->Initialize(device);
		cubeMap->LoadTexture("CloudCubeMap.dds");
		_cubeMaps["CloudCubeMap"] = std::move(cubeMap);
	}

	void ObjectManager::Finalize()
	{
		_fpsText.reset();
		_axis.reset();
		_grid.reset();
		_lineRenderer.reset();

		for (auto& cam : _cameraList)
		{
			delete cam;
		}

		for (auto& mr : _staticModelRendererList)
		{
			delete mr;
		}

		for (auto& dmr : _dynamicModelRendererList)
		{
			delete dmr;
		}

		for (auto& tr : _textList)
		{
			delete tr;
		}

		for (auto& sr : _spriteList)
		{
			delete sr;
		}

		for (auto& dl : _directionalLightList)
		{
			delete dl;
		}

		for (auto& iter : _cubeMaps)
		{
			iter.second.reset();
		}
	}

	Camera* ObjectManager::CreateCamera()
	{
		Camera* temp = new Camera();
		_cameraList.emplace_back(temp);

		return temp;
	}

	MeshRenderer* ObjectManager::CreateStaticModelRenderer()
	{
		MeshRenderer* meshRenderer = new MeshRenderer();

		// TODO : 기본 Mesh를 넣어주기로 했는데 이거 일단 보류.
		// meshRenderer->LoadModel(_resourceManager.GetCubeMesh());

		//skinnedMeshRenderer->LoadMesh(_resourceManager.GetCubeMesh());
		// TODO : 기본 Material을 넣어주고 앞단에서 Material을 바꿔서 넣어줄 수 있도록 하자
		//meshRenderer->SetMaterial(_resourceManager.GetDefaultMaterial());
		Material* material = new Material();
		material->SetTexture(_resourceManager.GetDefaultTexture());
		material->SetVertexShader(_resourceManager.GetVertexShader("StaticMeshVS"));
		material->SetPixelShader(_resourceManager.GetPixelShader("StaticMeshPS"));
		material->SetRenderState(_resourceManager.GetRenderState(ResourceManager::eRenderState::SOLID));
		meshRenderer->SetMaterial(material);
		
		_staticModelRendererList.emplace_back(meshRenderer);

		return meshRenderer;
	}

	Rocket::Core::SpriteRenderer* ObjectManager::CreateImage()
	{
		SpriteRenderer* temp = new SpriteRenderer();
		temp->SetImage("test.jpg");
		_spriteList.emplace_back(temp);

		return temp;
	}

	std::vector<SpriteRenderer*>& ObjectManager::GetImageList()
	{
		return _spriteList;
	}

	std::vector<MeshRenderer*>& ObjectManager::GetStaticModelRenderers()
	{
		return _staticModelRendererList;
	}

	Rocket::Core::TextRenderer* ObjectManager::CreateText()
	{
		TextRenderer* textRenderer = new TextRenderer();
		textRenderer->SetFont(ResourceManager::Instance().GetDefaultFont());
		_textList.emplace_back(textRenderer);
		return textRenderer;
	}

	std::vector<TextRenderer*>& ObjectManager::GetTextList()
	{
		return _textList;
	}

	LineRenderer* ObjectManager::CreateLineRenderer()
	{
		_lineRenderer = std::make_unique<LineRenderer>();
		return _lineRenderer.get();
	}

	LineRenderer* ObjectManager::GetLineRenderer()
	{
		return _lineRenderer.get();
	}

	std::vector<DynamicModelRenderer*>& ObjectManager::GetDynamicModelRenderers()
	{
		return _dynamicModelRendererList;
	}

	DynamicModelRenderer* ObjectManager::CreateDynamicModelRenderer()
	{
		DynamicModelRenderer* dynamicModelRenderer = new DynamicModelRenderer();

		//skinnedMeshRenderer->LoadMesh(_resourceManager.GetCubeMesh());
		// TODO : 기본 Material을 넣어주고 앞단에서 Material을 바꿔서 넣어줄 수 있도록 하자
		//skinnedMeshRenderer->SetMaterial(_resourceManager.GetDefaultMaterial());
		Material* material = new Material();
		material->SetTexture(_resourceManager.GetDefaultTexture());
		material->SetVertexShader(_resourceManager.GetVertexShader("SkinnedMeshVS"));
		material->SetPixelShader(_resourceManager.GetPixelShader("SkinnedMeshPS"));
		material->SetRenderState(_resourceManager.GetRenderState(ResourceManager::eRenderState::SOLID));
		dynamicModelRenderer->SetMaterial(material);

		_dynamicModelRendererList.emplace_back(dynamicModelRenderer);

		return dynamicModelRenderer;
	}

	DirectionalLight* ObjectManager::CreateDirectionalLight()
	{
		DirectionalLight* directionalLight = new DirectionalLight();
		_directionalLightList.emplace_back(directionalLight);

		return directionalLight;
	}

	std::vector<DirectionalLight*>& ObjectManager::GetDirectionalLightList()
	{
		return _directionalLightList;
	}

	Rocket::Core::CubeMap* ObjectManager::GetCubeMap(const std::string& name)
	{
		if(_cubeMaps.find(name) == _cubeMaps.end())
		{
			return nullptr;
		}

		return _cubeMaps.at(name).get();
	}

	Rocket::Core::CubeMap* ObjectManager::GetDefaultCubeMap()
	{
		return _cubeMaps.begin()->second.get();
	}
}
