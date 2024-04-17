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
		void AddGameObject(GameObject* gameObject);
		void DestroyGameObject(GameObject& gameObject);
		void Update(float deltaSecond);
		void EndRender();
		void StartFrame();

		std::vector<GameObject*> GetGameObjects() const { return _gameObjects; }

	private:
		std::vector<GameObject*> _gameObjects;
		std::list<std::pair<GameObject*, bool>> _gameObjectsToEnable;
		std::list<std::pair<Component*, bool>> _componentsToEnable;
		Timer _timer;
	};
}
