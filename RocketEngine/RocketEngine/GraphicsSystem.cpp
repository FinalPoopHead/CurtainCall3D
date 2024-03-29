#include <cassert>
#include <algorithm>
#include <windows.h>

#include "GraphicsSystem.h"
#include "SceneSystem.h"
#include "TimeSystem.h"
#include "Scene.h"
#include "Camera.h"
#include "Component.h"


using GRAPHICS_CREATE_SIGNATURE = Rocket::Core::IDX11Renderer* (*)(void);
constexpr const char* GRAPHICS_CREATE_NAME = "CreateGraphicsInstance";

#define GRAPHICS_RELEASE_SIGNATURE void(*)(Rocket::Core::IDX11Renderer*)
#define GRAPHICS_RELEASE_NAME "ReleaseGraphicsInstance"

using FACTORY_CREATE_SIGNATURE = Rocket::Core::IFactory* (*)(void);
constexpr const char* FACTORY_CREATE_NAME = "CreateGraphicsObjectFactory";

#define FACTORY_RELEASE_SIGNATURE void(*)(Rocket::Core::IFactory*)
#define FACTORY_RELEASE_NAME "ReleaseFactory"

using GET_RESOURCEMANAGER_SIGNATURE = Rocket::Core::IResourceManager* (*)(void);
constexpr const char* GET_RESOURCEMANAGER_NAME = "GetResourceManager";

namespace Rocket::Core
{
	GraphicsSystem::GraphicsSystem()
		:_hWnd(), _screenWidth(), _screenHeight(),
		hGraphicsModule(LoadLibrary(GRAPHICSDLL_PATH)),
		_graphicsRenderer(), _factory()
	{
		DWORD error_code = GetLastError();
		assert(hGraphicsModule);

		auto a = GetProcAddress(hGraphicsModule, GRAPHICS_CREATE_NAME);
		error_code = GetLastError();

		_graphicsRenderer.reset((reinterpret_cast<GRAPHICS_CREATE_SIGNATURE>(GetProcAddress(hGraphicsModule, GRAPHICS_CREATE_NAME)))());
		_factory.reset((reinterpret_cast<FACTORY_CREATE_SIGNATURE>(GetProcAddress(hGraphicsModule, FACTORY_CREATE_NAME)))());
		_resourceManager = (reinterpret_cast<GET_RESOURCEMANAGER_SIGNATURE>(GetProcAddress(hGraphicsModule, GET_RESOURCEMANAGER_NAME)))();
	}

	void GraphicsSystem::Initialize(HWND hWnd, int screenWidth, int screenHeight, bool isEditor /*= false*/)
	{
		_hWnd = hWnd;
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_isEditor = isEditor;

		_graphicsRenderer->Initialize(static_cast<void*>(hWnd), screenWidth, screenHeight);
	}

	void GraphicsSystem::Finalize()
	{
		_graphicsRenderer->Finalize();
		reinterpret_cast<GRAPHICS_RELEASE_SIGNATURE>(GetProcAddress(hGraphicsModule, GRAPHICS_RELEASE_NAME))(_graphicsRenderer.release());
		reinterpret_cast<FACTORY_RELEASE_SIGNATURE>(GetProcAddress(hGraphicsModule, FACTORY_RELEASE_NAME))(_factory.release());
		FreeLibrary(hGraphicsModule);
	}

	void GraphicsSystem::OnResize(int width, int height)
	{
		/*		_rocketGraphics->Resize(width, height);*/
	}

	void GraphicsSystem::DrawProcess()
	{
		UpdateRenderData(); //SetRenderData + 전체 렌더 시작,
		_graphicsRenderer->Update(TimeSystem::GetDeltaTime(), TimeSystem::GetFrameRate());
		_graphicsRenderer->Render();
	}

	void GraphicsSystem::UpdateRenderData()
	{
		auto mainCam = SceneSystem::Instance().GetCurrentScene()->GetMainCamera();
		mainCam->UpdateRenderData();

		for (auto& renderComp : _renderableList)
		{
			renderComp->UpdateRenderData();
		}
	}

	int GraphicsSystem::GetScreenWidth() const
	{
		return _screenWidth;
	}

	int GraphicsSystem::GetScreenHeight() const
	{
		return _screenHeight;
	}

	Rocket::Core::IFactory* GraphicsSystem::GetFactory() const
	{
		return _factory.get();
	}

	void GraphicsSystem::AddToList(Component* comp)
	{
		_renderableList.emplace_back(comp);
	}

	void GraphicsSystem::DestroyWindow()
	{
		::DestroyWindow(_hWnd);
	}

	void GraphicsSystem::SetDebugMode(bool isDebug)
	{
		_graphicsRenderer->SetDebugMode(isDebug);
	}

// 	void GraphicsSystem::MakeRenderableAll()
// 	{
// 		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
// 		{
// 			if (sceneIter.second == nullptr)
// 			{
// 				return;
// 			}
// 
// 			for (auto& object : sceneIter.second->GetOriginalList())
// 			{
// 				// 각각의 객체가 본인들을 그리는 것이 아닌
// 				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
// 				// object->GetRenderData();
// 
// 				MeshRendererBase* renderer = object->GetComponent<Rocket::MeshRenderer>();
// 
// 				if (!renderer)
// 				{
// 					renderer = object->GetComponent<Rocket::SkinnedMeshRenderer>();
// 
// 					if (!renderer)
// 					{
// 						continue;
// 					}
// 				}
// 
// 				if (_renderMap.find(renderer) == _renderMap.end())
// 				{
// 					if (dynamic_cast<Rocket::MeshRenderer*>(renderer))
// 					{
// // 						Rocket::Core::IRenderable* renderableObj = _rocketGraphics->MakeRenderable(renderer->GetModelData());
// // 						_renderMap.insert({ renderer, renderableObj });
// 					}
// 
// 					if (dynamic_cast<Rocket::SkinnedMeshRenderer*>(renderer))
// 					{
// // 						Rocket::Core::IRenderable* renderableObj = _rocketGraphics->MakeSkinnedRenderable(renderer->GetModelData());
// // 						_renderMap.insert({ renderer, renderableObj });
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	void GraphicsSystem::MakeAnimationAll()
// 	{
// 		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
// 		{
// 			if (sceneIter.second == nullptr)
// 			{
// 				return;
// 			}
// 
// 			for (auto& object : sceneIter.second->GetOriginalList())
// 			{
// 				// 각각의 객체가 본인들을 그리는 것이 아닌
// 				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
// 				// object->GetRenderData();
// 
// 				Rocket::AnimationController* animCtr = object->GetComponent<Rocket::AnimationController>();
// 
// 				if (!animCtr)
// 				{
// 					continue;
// 				}
// 
// 				for (auto& iter : animCtr->GetAnimator()->GetAllStates())
// 				{
// /*					_rocketGraphics->MakeAnim(iter.second->GetAnimDesc(), &(iter.second->GetAnimDataRef()));*/
// 				}
// 			}
// 		}
// 	}
// 
// 	void GraphicsSystem::MakeUIAll()
// 	{
// 		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
// 		{
// 			if (sceneIter.second == nullptr)
// 			{
// 				return;
// 			}
// 
// 			for (auto& object : sceneIter.second->GetOriginalList())
// 			{
// 				// 각각의 객체가 본인들을 그리는 것이 아닌
// 				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
// 				// object->GetRenderData();
// 
// 				std::vector<UIRenderer*> uiVec = object->GetComponentsDynamic<UIRenderer>();
// 
// 				for (auto& ui : uiVec)
// 				{
// 					if (_uiMap.find(ui) == _uiMap.end())
// 					{
// 						Rocket::TextBox* tr = nullptr;
// 						if (tr = dynamic_cast<Rocket::TextBox*>(ui))
// 						{
// // 							Rocket::Core::ISketchable* sketchableObj = _rocketGraphics->MakeText(tr->GetFontSize(), tr->GetWidth(), tr->GetHeight(), tr->GetColor());
// // 							_uiMap.insert({ ui, sketchableObj });
// 						}
// 
// 						Rocket::SpriteRenderer* sr = nullptr;
// 						if (sr = dynamic_cast<Rocket::SpriteRenderer*>(ui))
// 						{
// // 							Rocket::Core::ISketchable* sketchableObj = _rocketGraphics->MakeImage(sr->GetPath().c_str(), sr->GetWidth(), sr->GetHeight());
// // 							_uiMap.insert({ ui, sketchableObj });
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}



// 	void GraphicsSystem::UpdateConstantData(Rocket::Core::RenderConstantData& data)
// 	{
// 		// TODO : 씬 전역적으로 관리하지말고 씬단위로 관리할것 ㄷㄷ;;
// 		// 지금 라이트가 씬에 따라 가져오지 않고 그냥 전역적으로 갖고온다 세상에..
// 		// 23.08.23 강석원 인재원.
// 
// 		for (auto& light : _lights)
// 		{
// 			if (!(light->gameObject->IsActive()))
// 			{
// 				continue;
// 			}
// 
// 			void* temp = nullptr;
// 
// 			temp = dynamic_cast<Rocket::DirectionalLight*>(light);
// 
// 			if (temp)
// 			{
// 				data.dirLightList.push_back(static_cast<Rocket::DirectionalLight*>(temp)->GetLightData());
// 				continue;
// 			}
// 
// 			temp = dynamic_cast<Rocket::PointLight*>(light);
// 
// 			if (temp)
// 			{
// 				data.pointLightList.push_back(static_cast<Rocket::PointLight*>(temp)->GetLightData());
// 				continue;
// 			}
// 
// 			temp = dynamic_cast<Rocket::SpotLight*>(light);
// 
// 			if (temp)
// 			{
// 				data.spotLightList.push_back(static_cast<Rocket::SpotLight*>(temp)->GetLightData());
// 				continue;
// 			}
// 		}
// 
// 		data.deltaTime = Rocket::Core::TimeSystem::Instance().GetDeltaTime();
// 	}
// 
// 
// 	void GraphicsSystem::DrawCurrentScene()
// {
// 		Rocket::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
// 
// 		// 현재 씬이 존재하지 않으면 Return.
// 		if (currentScene == nullptr)
// 		{
// 			return;
// 		}
// 
// 		/// 현재씬이 존재한다면 진행될 동작들.
// 		_rocketGraphics->UpdateCamera(currentScene->GetMainCamera()->GetCameraData());
// 
// 		for (auto& object : currentScene->GetOriginalList())
// 		{ 
// 			// 각각의 객체가 본인들을 그리는 것이 아닌
// 			// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
// 			// object->GetRenderData();
// 
// 			if (!object->IsActive())
// 			{
// 				continue;
// 			}
// 
// 			MeshRendererBase* renderer = object->GetComponentDynamic<MeshRendererBase>();
// 
// 			if (!renderer)
// 			{
// 				continue;
// 			}
// 
// 			// 위험하지만 우선 부하때문에 주석처리..?
// // 			if (_renderMap.find(renderer) == _renderMap.end())
// // 			{
// // 				Rocket::Core::IRenderable* renderableObj = _rocketGraphics->MakeRenderable(renderer->GetModelData());
// // 				_renderMap.insert({ renderer, renderableObj });
// // 			}
// 
// 		}
// 	}
// 
// 	void GraphicsSystem::DrawCurrentUI()
// 	{
// 		Rocket::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
// 
// 		// 현재 씬이 존재하지 않으면 Return.
// 		if (currentScene == nullptr)
// 		{
// 			return;
// 		}
// 
// 		std::vector<UIRenderer*> UIVec;
// 
// 		for (auto& object : currentScene->GetOriginalList())
// 		{
// 			// 각각의 객체가 본인들을 그리는 것이 아닌
// 			// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
// 			// object->GetRenderData();
// 
// 			if (!object->IsActive())
// 			{
// 				continue;
// 			}
// 
// 			std::vector<UIRenderer*> renderers = object->GetComponentsDynamic<UIRenderer>();
// 
// 
// 			for (auto& renderer : renderers)
// 			{
// 				// button일때.. button은 sketchable을 만들지 않는다.
// 				if (dynamic_cast<Rocket::Button*>(renderer))
// 				{
// 					continue;
// 				}
// 
// 				UIVec.push_back(renderer);
// 
// 
// 			}
// 		}
// 
// 		std::sort(UIVec.begin(), UIVec.end(),
// 			[](UIRenderer* lhs, UIRenderer* rhs) 
// 			{
// 				return lhs->GetSortOrder() < rhs->GetSortOrder();
// 			});
// 
// 		for (auto& renderer : UIVec)
// 		{
// 			Rocket::Core::ISketchable* temp = _uiMap.at(renderer);
// 			temp->SetData(renderer->GetSketchData().c_str());
// 			temp->SetFillRatio(renderer->GetFillRatio());
//  			_uiMap.at(renderer)->Render(renderer->Get2DWorldTM());
// 		}
// 		
// 	}

// 	void RenderSystem::DrawDebugBox()
// 	{
// 		if (!(DebugSystem::Instance().IsDebugMode() || _isEditor))
// 		{
// 			return;
// 		}
// 
// 		for (auto& box : DebugSystem::Instance().GetBoxQueue())
// 		{
// 			//_rocketGraphics->RenderCube(box.worldTM, box.widthHeightDepth.x, box.widthHeightDepth.y, box.widthHeightDepth.z, { box.isWire,box.color.r,box.color.g,box.color.b,box.color.a });
// 		}
// 
// 		DebugSystem::Instance().FlushBoxQueue();
// 	}

// 	void RenderSystem::DrawDebugText()
// 	{
// 		if (!(DebugSystem::Instance().IsDebugMode()))
// 		{
// 			return;
// 		}
// 
// 		DebugSystem::Instance().DrawDebugText({ 50.0f,50.0f }, "FPS : " + std::to_string(TimeSystem::Instance().GetFrameRate()), 20.0f);
// 
// 		for (auto& text : DebugSystem::Instance().GetTextQueue())
// 		{
// 			//_rocketGraphics->RenderDebugText(text.centerPos.x, text.centerPos.y, text.content.c_str(), text.size);
// 		}
// 
// 		DebugSystem::Instance().FlushTextQueue();
// 	}
// 
// 	void RenderSystem::DrawDebugLine()
// 	{
// 		if (!(DebugSystem::Instance().IsDebugMode()))
// 		{
// 			return;
// 		}
// 
// 		for (auto& line : DebugSystem::Instance().GetLineQueue())
// 		{
// 			//_rocketGraphics->RenderLine(line.beginPoint, line.endPoint, line.color);
// 		}
// 
// 		DebugSystem::Instance().FlushLineQueue();
// 	}
// 
// 	void RenderSystem::DrawDebug2DBox()
// 	{
// 		if (!(DebugSystem::Instance().IsDebugMode()))
// 		{
// 			return;
// 		}
// 
// 		for (auto& box2D : DebugSystem::Instance().Get2DBoxQueue())
// 		{
// 			//_rocketGraphics->Render2DBox(box2D.LT, box2D.RB, box2D.color);
// 		}
// 
// 		DebugSystem::Instance().Flush2DBoxQueue();
// 	}
// 
// 	void RenderSystem::RenderPhysicsDebug()
// 	{
// 		PhysicsSystem& temp = PhysicsSystem::Instance();
// 		for (auto& rigid : temp.GetRigidDynamicsList())
// 		{
// 			Rocket::DynamicCollider* col = static_cast<Rocket::DynamicCollider*>(rigid->userData);
// 			Rocket::BoxCollider* temp = dynamic_cast<Rocket::BoxCollider*>(col);
// 			Rocket::Matrix worldTM = col->gameObject->transform.GetWorldTM();
// 			if (temp)
// 			{
// 				//_rocketGraphics->RenderCube(worldTM, temp->GetWidth(), temp->GetHeight(), temp->GetDepth(), { true,1.0f,0.0f,0.0f,1.0f });
// 			}
// 			else
// 			{
// 				//_rocketGraphics->RenderCube(worldTM, { true,1.0f,0.0f,0.0f,1.0f });
// 			}
// 		}
// 
// 		for (auto& rigid : temp.GetRigidStaticsList())
// 		{
// 			Rocket::StaticCollider* col = static_cast<Rocket::StaticCollider*>(rigid->userData);
// 			Rocket::PlaneCollider* temp = dynamic_cast<Rocket::PlaneCollider*>(col);
// 			Rocket::Matrix worldTM = col->gameObject->transform.GetWorldTM();
// 			if (temp)
// 			{
// 				//_rocketGraphics->RenderCube(worldTM, 0.01f, 100.0f, 100.0f, { false,0.0f,1.0f,0.0f,1.0f });
// 			}
// 			else
// 			{
// 			//	_rocketGraphics->RenderCube(worldTM, { true,1.0f,0.0f,0.0f,1.0f });
// 			}
// 		}
// 	}
}
