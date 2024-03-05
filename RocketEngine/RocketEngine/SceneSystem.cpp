#include "SceneSystem.h"
#include "Scene.h"
#include <cassert>
#include "SoundSystem.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

namespace Rocket::Core
{
	SceneSystem::SceneSystem()
		:_currentScene(nullptr),
		_sceneMap()
	{

	}

	Rocket::Scene* SceneSystem::GetCurrentScene()
	{
		return _currentScene;
	}

	std::unordered_map<std::string, Rocket::Scene*>& SceneSystem::GetAllScenes()
	{
		return _sceneMap;
	}

	Rocket::Scene* SceneSystem::CreateScene(const std::string& sceneName, bool withNoCam /*= false*/)
	{
		auto iter = _sceneMap.find(sceneName);

		if (iter != _sceneMap.end())
		{
			return _sceneMap[sceneName];
		}

		Rocket::Scene* scene;

		scene = new Rocket::Scene(sceneName, withNoCam);


		_sceneMap.insert({ sceneName, scene });

		// 원래는 LoadScene을 추가로 해줬었는데
		// LoadScene의 메커니즘 변경으로 씬 생성시 바로 적용시키는 것으로.
		// 
		// 23.08.16 강석원 집.
// 		if (_currentScene == nullptr)
// 		{
// 			_currentScene = scene;
// 		}

		_currentScene = scene;

		return scene;
	}

	void SceneSystem::Initialize()
	{
	}

	void SceneSystem::Finalize()
	{
		for (auto& iter : _sceneMap)
		{
			delete iter.second;
		}
	}

	/// 로드할 씬이 있으면 변경하고 초기화한다.
	/// 사운드 리스너도 설정한다.
	/// 
	/// 23.08.16 강석원 집.
	void SceneSystem::UpdateLoadScene()
	{
		if (!_loadScene)
		{
			return;
		}

		_currentScene = _loadScene;
		_currentScene->Initialize();
		SoundSystem::Instance().SetListenerTransform(&(_currentScene->GetMainCamera()->gameObject->transform));

		_loadScene = nullptr;
	}

	/// 씬을 로드한다.
	/// 바로 변경하지 않고, 다음 프레임이 시작할 때 변경한다.
	/// 변경하면서 초기화도 진행한다.
	/// 
	/// 23.08.16 강석원 집.
	bool SceneSystem::LoadScene(const std::string& sceneName)
	{
		auto iter = _sceneMap.find(sceneName);
		if (iter == _sceneMap.end())
		{
			// 존재하지 않는 씬입니다.
			return false;
		}

		// 찾은 씬의 주소를 넘긴다.
		//_currentScene->Finalize();
		_loadScene = iter->second;
		return true;
	}

}