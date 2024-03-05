#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
#include <cassert>

namespace Rocket
{
	class Scene;
}

namespace Rocket::Core
{
	class SceneSystem : public Singleton<SceneSystem>
	{
		friend Singleton;
	private:
		SceneSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

	public:
		void Initialize();
		void Finalize();
		void UpdateLoadScene();

	public:
		Rocket::Scene* GetCurrentScene();
		std::unordered_map<std::string, Rocket::Scene*>& GetAllScenes();

		/// RocketAPI ���� �����ؼ� ����ϴ� �Լ���.
	public:
		Rocket::Scene* CreateScene(const std::string& sceneName, bool withNoCam = false);		
		bool LoadScene(const std::string& sceneName);

	private:
		Rocket::Scene* _loadScene;
		Rocket::Scene* _currentScene;
		std::unordered_map<std::string, Rocket::Scene*> _sceneMap;
	};
}
