#pragma once
#include "./internal/GameEngine.h"
#include "./internal/Scene.h"
#include "./internal/GameObject.h"
#include "./internal/BuiltinComponent.h"

namespace flt
{
	template<GameObjectType T, typename... TArgs>
	T* CreateGameObject(bool isEnabled, TArgs&&... args)
	{
		GameEngine* engine = GameEngine::Instance();
		Scene* scene = engine->GetCurrentScene();
		return scene->InstantiateGameObject<T>(isEnabled, std::forward<TArgs>(args)...);
	}
}
