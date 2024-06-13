#pragma once
#include "./internal/Scene.h"
#include "./internal/GameObject.h"
#include "./internal/BuiltinComponent.h"
#include "../../FloaterPlatform/include/KeyCode.h"
#include "../../FloaterPlatform/include/GamePad.h"
#include "../../FloaterMath/include/floaterMath.h"

#include "../../FloaterUtil/include/Type.h"


namespace flt
{
	class GameEngine;

	struct GameEngineWrapper
	{
		GameEngineWrapper();

		flt::Scene* GetCurrentScene();
		bool AddScene(const std::wstring& sceneName, flt::Scene* scene);

		GameEngine* engine;
	};

	namespace __impl
	{
		extern GameEngineWrapper g_engineWrapper;
	}

	template<GameObjectDerived T, typename... TArgs>
	T* CreateGameObject(bool isEnabled, TArgs&&... args)
	{
		Scene* scene = __impl::g_engineWrapper.GetCurrentScene();
		return scene->InstantiateGameObject<T>(isEnabled, std::forward<TArgs>(args)...);
	}

	template<SceneDerived T, typename... TArgs>
	T* CreateScene(TArgs&&... args)
	{
		T* scene = new T(std::forward<TArgs>(args)...);
		std::wstring sceneName = flt::TypeName<T>().data();
		sceneName.resize(flt::TypeName<T>().length());
		__impl::g_engineWrapper.AddScene(sceneName, scene);
		return scene;
	}

	Scene* SetScene(Scene* scene);

	Scene* SetScene(const std::wstring& sceneName);

	Vector2f GetWindowSize();

	void ExitGame();
}
