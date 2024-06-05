﻿#pragma once
#include "GameObject.h"
#include "Component.h"
#include "../../CollisionPair.h"
#include "../../../FloaterUtil/include/Timer.h"
#include <unordered_set>
#include <list>
#include <vector>
#include <set>
#include <string>

namespace flt
{
	template<GameObjectDerived T, typename... TArgs>
	T* CreateGameObject(bool isEnabled, TArgs&&... args);

	class Scene
	{
		friend class GameObject;
		friend class GameEngine;

		template<GameObjectDerived T, typename... TArgs>
		friend T* flt::CreateGameObject(bool isEnabled, TArgs&&... args);

	public:
		Scene();
		~Scene();

		virtual void Initialize();
		virtual void Finalize();

		//[[deprecated("Use template CreateGameObject instead")]] void CreateGameObject(GameObject* gameObject);
		void PrePhysicsUpdate();
		void PostPhysicsUpdate();
		void Update(float deltaSecond);
		void EndRender();
		void StartFrame();
		void EndFrame();


		std::vector<GameObject*> GetGameObjects() const { return _gameObjects; }
		std::vector<GameObject*> GetGameObjects(const std::wstring& name) const;

	private:
		void AddEnableGameObject(GameObject* gameObject, bool isEnable);
		void AddDestroyGameObject(GameObject* gameObject);

	private:
		template<GameObjectDerived T, typename... TArgs>
		T* InstantiateGameObject(bool isEnabled, TArgs&&... args);

		void CallCollisionEvent();

	private:
		std::vector<GameObject*> _gameObjects;
		std::vector<GameObject*> _gameObjectsToCreate;
		std::vector<GameObject*> _gameObjectsToEnable;
		std::vector<GameObject*> _gameObjectsToDisable;
		std::vector<GameObject*> _gameObjectsToDestroy;

		std::vector<ComponentBase*> _componentsToEnable;
		std::vector<ComponentBase*> _componentsToDisable;

		std::vector<CollisionPair> _collisionPairs;
		//std::unordered_set<CollisionPair> _collisionSet;
		bool _collisionFlag = false;
	};

	template<GameObjectDerived T, typename... TArgs>
	T* Scene::InstantiateGameObject(bool isEnabled, TArgs&&... args)
	{
		GameObject* gameObject = new T(std::forward<TArgs>(args)...);
		gameObject->_scene = this;
		gameObject->_isEnable = isEnabled;

		_gameObjectsToCreate.emplace_back(gameObject);
		//_gameObjects.emplace_back(gameObject);
		//if (isEnabled)
		//{
		//	_gameObjectsToEnable.emplace_back(gameObject);
		//}

		return static_cast<T*>(gameObject);
	}

	template <typename T>
	concept SceneDerived = requires(T a)
	{
		std::is_base_of_v<flt::Scene, T>;
	};
}
