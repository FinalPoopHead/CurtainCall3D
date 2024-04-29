#pragma once
#include "GameObject.h"
#include "Component.h"
#include "../../FloaterUtil/include/Timer.h"
#include <unordered_map>
#include <list>
#include <vector>

namespace flt
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		virtual void Initialize();
		virtual void Finalize();
		GameObject* CreateGameObject();

		[[deprecated("Use template AddGameObject instead")]] void AddGameObject(GameObject* gameObject);
		void DestroyGameObject(GameObject& gameObject);
		void Update(float deltaSecond);
		void EndRender();
		void StartFrame();

		template<GameObjectType T>
		T* AddGameObject();

		std::vector<GameObject*> GetGameObjects() const { return _gameObjects; }

	private:
		std::vector<GameObject*> _gameObjects;
		std::list<std::pair<GameObject*, bool>> _gameObjectsToEnable;
		std::list<std::pair<Component*, bool>> _componentsToEnable;

		float _fixedUpdateElapsedSecond;
	};

	template<GameObjectType T>
	T* flt::Scene::AddGameObject()
	{
		GameObject* gameObject = new T();

		_gameObjects.emplace_back(gameObject);
		if (gameObject->_isEnable)
		{
			_gameObjectsToEnable.emplace_back(gameObject, gameObject->_isEnable);

			for (auto& component : gameObject->_components)
			{
				if (component == nullptr)
				{
					continue;
				}

				_componentsToEnable.emplace_back(component, true);
			}
		}

		return static_cast<T*>(gameObject);
	}
}
