﻿#include "ObjectManager.h"

#include "..\\RocketCommon\\IGraphicsObject.h"

#include "ResourcePath.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "DynamicModelRenderer.h"
#include "StaticModelRenderer.h"
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
		_meshRendererList(),
		_cameraList(),
		_textList()
	{
	}

	void ObjectManager::Initialize(ID3D11Device* device)
	{
		_debugText.reset(new TextRenderer());
		_debugText->SetFont(_resourceManager.GetDefaultFont());

		_axis.reset(new Axis());
		_axis->Initialize(device);
		_axis->SetRenderState(_resourceManager.GetRenderState(ResourceManager::eRenderState::WIREFRAME));
		_axis->SetShader(_resourceManager.GetVertexShader("ColorVS"), _resourceManager.GetPixelShader("ColorPS")); // Forward
		// _axis->SetShader(_rscMgr.GetVertexShader("DeferredColorVS"), _rscMgr.GetPixelShader("DeferredColorPS")); // Deferred

		_grid.reset(new Grid());
		_grid->Initialize(device);
		_grid->SetRenderState(_resourceManager.GetRenderState(ResourceManager::eRenderState::WIREFRAME));
		_grid->SetShader(_resourceManager.GetVertexShader("ColorVS"), _resourceManager.GetPixelShader("ColorPS")); // Forward
		// _grid->SetShader(_resourceManager.GetVertexShader("DeferredColorVS"), _resourceManager.GetPixelShader("DeferredColorPS")); // Deferred

		_cubeMap = std::make_unique<CubeMap>();
		_cubeMap->Initialize(device, _resourceManager.GetDeviceContext());
		_cubeMap->SetShader(_resourceManager.GetVertexShader("CubeMapVS"), _resourceManager.GetPixelShader("CubeMapPS")); // Forward
		_cubeMap->SetIBLGenShader(_resourceManager.GetPixelShader("IrradianceMapPS"), _resourceManager.GetPixelShader("PrefilteredMapPS"), _resourceManager.GetPixelShader("BRDF2DLUTPS"));
		// cubeMap->SetShader(_resourceManager.GetVertexShader("DeferredCubeMapVS"), _resourceManager.GetPixelShader("DeferredCubeMapPS")); // Deferred
		//_cubeMap->LoadTexture("Garden.dds");
		_cubeMap->LoadTexture("CloudCubeMap.dds");
		//_cubeMap->LoadTexture("CobblestoneStreet.dds");
	}

	void ObjectManager::Finalize()
	{
		_debugText.reset();
		_axis.reset();
		_grid.reset();
		_lineRenderer.reset();

		for (auto& cam : _cameraList)
		{
			delete cam;
		}

		for (auto& mr : _meshRendererList)
		{
			delete mr;
		}

		for (auto& dmr : _dynamicModelRendererList)
		{
			delete dmr;
		}

		for (auto& smr : _staticModelRendererList)
		{
			delete smr;
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

		_cubeMap.reset();
	}

	Camera* ObjectManager::CreateCamera()
	{
		Camera* temp = new Camera();
		_cameraList.emplace_back(temp);

		return temp;
	}

	MeshRenderer* ObjectManager::CreateMeshRenderer()
	{
		MeshRenderer* meshRenderer = new MeshRenderer();

		// TODO : 기본 Mesh를 넣어주기로 했는데 이거 일단 보류.
		// meshRenderer->LoadModel(_resourceManager.GetCubeMesh());

		// TODO : 기본 Material을 넣어주고 앞단에서 Material을 바꿔서 넣어줄 수 있도록 하자
		//meshRenderer->SetMaterial(_resourceManager.GetDefaultMaterial());
		Material* material = new Material();
		material->SetBaseColorTexture(_resourceManager.GetDefaultTexture());

		// TODO : 디퍼드 셰이더를 여기서 수동으로 바꿔주는게 조금 아쉽다.
// 		material->SetVertexShader(_resourceManager.GetVertexShader("StaticMeshVS"));
// 		material->SetPixelShader(_resourceManager.GetPixelShader("StaticMeshPS"));
		material->SetVertexShader(_resourceManager.GetVertexShader("DeferredStaticMeshVS"));
		material->SetPixelShader(_resourceManager.GetPixelShader("DeferredStaticMeshPS"));
		material->SetRenderState(_resourceManager.GetRenderState(ResourceManager::eRenderState::SOLID));
		meshRenderer->SetMaterial(material);
		
		_meshRendererList.emplace_back(meshRenderer);

		return meshRenderer;
	}

	Rocket::Core::SpriteRenderer* ObjectManager::CreateImage()
	{
		SpriteRenderer* temp = new SpriteRenderer();
		temp->SetImage(TEXTURE_PATH + "test.jpg");
		_spriteList.emplace_back(temp);

		return temp;
	}

	std::vector<SpriteRenderer*>& ObjectManager::GetImageList()
	{
		return _spriteList;
	}

	std::vector<MeshRenderer*>& ObjectManager::GetMeshRenderers()
	{
		return _meshRendererList;
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
		material->SetBaseColorTexture(_resourceManager.GetDefaultTexture());
		// TODO : 디퍼드 셰이더를 여기서 수동으로 바꿔주는게 조금 아쉽다.
// 		material->SetVertexShader(_resourceManager.GetVertexShader("SkinnedMeshVS"));
// 		material->SetPixelShader(_resourceManager.GetPixelShader("SkinnedMeshPS"));
		material->SetVertexShader(_resourceManager.GetVertexShader("DeferredSkinnedMeshVS"));
		material->SetPixelShader(_resourceManager.GetPixelShader("DeferredSkinnedMeshPS"));
		material->SetRenderState(_resourceManager.GetRenderState(ResourceManager::eRenderState::SOLID));
		dynamicModelRenderer->SetMaterial(material);

		_dynamicModelRendererList.emplace_back(dynamicModelRenderer);

		return dynamicModelRenderer;
	}


	StaticModelRenderer* ObjectManager::CreateStaticModelRenderer()
	{
		StaticModelRenderer* staticModelRenderer = new StaticModelRenderer();

		// TODO : 기본 Mesh를 넣어주기로 했는데 이거 일단 보류.
		// meshRenderer->LoadModel(_resourceManager.GetCubeMesh());

		// TODO : 기본 Material을 넣어주고 앞단에서 Material을 바꿔서 넣어줄 수 있도록 하자
		//meshRenderer->SetMaterial(_resourceManager.GetDefaultMaterial());
		Material* material = new Material();
		material->SetBaseColorTexture(_resourceManager.GetDefaultTexture());

		// TODO : 디퍼드 셰이더를 여기서 수동으로 바꿔주는게 조금 아쉽다.
// 		material->SetVertexShader(_resourceManager.GetVertexShader("StaticMeshVS"));
// 		material->SetPixelShader(_resourceManager.GetPixelShader("StaticMeshPS"));
		material->SetVertexShader(_resourceManager.GetVertexShader("DeferredStaticMeshVS"));
		material->SetPixelShader(_resourceManager.GetPixelShader("DeferredStaticMeshPS"));
		material->SetRenderState(_resourceManager.GetRenderState(ResourceManager::eRenderState::SOLID));
		staticModelRenderer->SetMaterial(material);

		_staticModelRendererList.emplace_back(staticModelRenderer);

		return staticModelRenderer;
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

	Rocket::Core::CubeMap* ObjectManager::GetCubeMap()
	{

		return _cubeMap.get();
	}

	std::vector<StaticModelRenderer*>& ObjectManager::GetStaticModelRenderers()
	{
		return _staticModelRendererList;
	}

	void ObjectManager::DestroyCamera(Camera* camera, bool isLastMainCamera /*= false*/)
	{
		auto iter = std::find(_cameraList.begin(), _cameraList.end(), camera);
		if (iter != _cameraList.end())
		{
			_cameraList.erase(iter);
			delete camera;
		}

		if (isLastMainCamera)
		{
			if (!_cameraList.empty())
			{
				// 모든 카메라가 없는 경우
				// 그런 경우가 있을 수 있는데 앞단에서 Render를 호출 안해주기로 했음.
			}
			else
			{
				_cameraList.front()->SetAsMainCamera();
			}
		}
	}

	void ObjectManager::DestroyMeshRenderer(MeshRenderer* meshRenderer)
	{
		auto iter = std::find(_meshRendererList.begin(), _meshRendererList.end(), meshRenderer);
		if (iter != _meshRendererList.end())
		{
			_meshRendererList.erase(iter);
			delete meshRenderer;
		}
	}

	void ObjectManager::DestroyDynamicModelRenderer(DynamicModelRenderer* dynamicModelRenderer)
	{
		auto iter = std::find(_dynamicModelRendererList.begin(), _dynamicModelRendererList.end(), dynamicModelRenderer);
		if (iter != _dynamicModelRendererList.end())
		{
			_dynamicModelRendererList.erase(iter);
			delete dynamicModelRenderer;
		}
	}

	void ObjectManager::DestroyStaticModelRenderer(StaticModelRenderer* staticModelRenderer)
	{
		auto iter = std::find(_staticModelRendererList.begin(), _staticModelRendererList.end(), staticModelRenderer);
		if (iter != _staticModelRendererList.end())
		{
			_staticModelRendererList.erase(iter);
			delete staticModelRenderer;
		}
	}

	void ObjectManager::DestroyTextRenderer(TextRenderer* textRenderer)
	{
		auto iter = std::find(_textList.begin(), _textList.end(), textRenderer);
		if (iter != _textList.end())
		{
			_textList.erase(iter);
			delete textRenderer;
		}
	}

	void ObjectManager::DestroySpriteRenderer(SpriteRenderer* spriteRenderer)
	{
		auto iter = std::find(_spriteList.begin(), _spriteList.end(), spriteRenderer);
		if (iter != _spriteList.end())
		{
			_spriteList.erase(iter);
			delete spriteRenderer;
		}
	}

	void ObjectManager::DestroyDirectionalLight(DirectionalLight* directionalLight)
	{
		auto iter = std::find(_directionalLightList.begin(), _directionalLightList.end(), directionalLight);
		if (iter != _directionalLightList.end())
		{
			_directionalLightList.erase(iter);
			delete directionalLight;
		}
	}
}
