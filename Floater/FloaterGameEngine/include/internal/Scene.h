#pragma once
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
	template<GameObjectType T, typename... TArgs>
	T* CreateGameObject(bool isEnabled, TArgs&&... args);

	class Scene
	{
		friend class GameObject;
		friend class GameEngine;

		template<GameObjectType T, typename... TArgs>
		friend T* flt::CreateGameObject(bool isEnabled, TArgs&&... args);

	public:
		Scene();
		~Scene();

		virtual void Initialize();
		virtual void Finalize();

		//[[deprecated("Use template CreateGameObject instead")]] void CreateGameObject(GameObject* gameObject);
		void DestroyGameObject(GameObject& gameObject);
		void PrePhysicsUpdate();
		void PostPhysicsUpdate();
		void Update(float deltaSecond);
		void EndRender();
		void StartFrame();


		std::vector<GameObject*> GetGameObjects() const { return _gameObjects; }
		std::vector<GameObject*> GetGameObjects(const std::wstring& name) const;

	private:
		template<GameObjectType T, typename... TArgs>
		T* InstantiateGameObject(bool isEnabled, TArgs&&... args);

		void CallCollisionEvent();

	private:
		void Collision(GameObject* gameObject);

	private:
		std::vector<GameObject*> _gameObjects;
		std::list<std::pair<GameObject*, bool>> _gameObjectsToEnable;
		std::list<std::pair<ComponentBase*, bool>> _componentsToEnable;

		std::vector<CollisionPair> _collisionPairs;
		//std::unordered_set<CollisionPair> _collisionSet;
		bool _collisionFlag = false;
	};

	template<GameObjectType T, typename... TArgs>
	T* Scene::InstantiateGameObject(bool isEnabled, TArgs&&... args)
	{
		GameObject* gameObject = new T(std::forward<TArgs>(args)...);
		gameObject->_isEnable = false;

		_gameObjects.emplace_back(gameObject);
		if (isEnabled)
		{
			_gameObjectsToEnable.emplace_back(gameObject, true);
		}

		return static_cast<T*>(gameObject);
	}
}
