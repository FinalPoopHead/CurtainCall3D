#pragma once
#include "GameObject.h"
#include "Component.h"
#include "../../FloaterUtil/include/Timer.h"
#include <unordered_map>
#include <list>
#include <vector>
#include <string>

namespace flt
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		virtual void Initialize();
		virtual void Finalize();

		[[deprecated("Use template CreateGameObject instead")]] void CreateGameObject(GameObject* gameObject);
		void DestroyGameObject(GameObject& gameObject);
		void PrePhysicsUpdate();
		void PostPhysicsUpdate();
		void Update(float deltaSecond);
		void EndRender();
		void StartFrame();

		template<GameObjectType T>
		T* CreateGameObject(bool isEnabled = true);

		std::vector<GameObject*> GetGameObjects() const { return _gameObjects; }
		std::vector<GameObject*> GetGameObjects(const std::wstring& name) const;

	private:
		std::vector<GameObject*> _gameObjects;
		std::list<std::pair<GameObject*, bool>> _gameObjectsToEnable;
		std::list<std::pair<ComponentBase*, bool>> _componentsToEnable;
	};

	template<GameObjectType T>
	T* Scene::CreateGameObject(bool isEnabled /*= true*/)
	{
		GameObject* gameObject = new T();
		gameObject->_isEnable = false;

		_gameObjects.emplace_back(gameObject);
		if (isEnabled)
		{
			_gameObjectsToEnable.emplace_back(gameObject, true);
		}

		return static_cast<T*>(gameObject);
	}

}
