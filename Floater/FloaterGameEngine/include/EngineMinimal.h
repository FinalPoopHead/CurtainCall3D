#pragma once
#include "./internal/Scene.h"
#include "./internal/GameObject.h"
#include "./internal/BuiltinComponent.h"

namespace flt
{
	class GameEngine;

	struct GameEngineWrapper
	{
		GameEngineWrapper();

		flt::Scene* GetCurrentScene();
		void AddScene(flt::Scene* scene);

		GameEngine* engine;
	};

	namespace __impl
	{
		extern GameEngineWrapper g_engine;
	}

	template<GameObjectDerived T, typename... TArgs>
	T* CreateGameObject(bool isEnabled, TArgs&&... args)
	{
		Scene* scene = __impl::g_engine.GetCurrentScene();
		return scene->InstantiateGameObject<T>(isEnabled, std::forward<TArgs>(args)...);
	}

	template<SceneDerived T, typename... TArgs>
	T* CreateScene(TArgs&&... args)
	{
		T* scene = new T(std::forward<TArgs>(args)...);
		__impl::g_engine.AddScene(scene);
		return scene;
	}

	Scene* SetScene(Scene* scene);
}
