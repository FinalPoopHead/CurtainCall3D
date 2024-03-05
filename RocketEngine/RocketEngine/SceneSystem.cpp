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

		// ������ LoadScene�� �߰��� ������µ�
		// LoadScene�� ��Ŀ���� �������� �� ������ �ٷ� �����Ű�� ������.
		// 
		// 23.08.16 ������ ��.
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

	/// �ε��� ���� ������ �����ϰ� �ʱ�ȭ�Ѵ�.
	/// ���� �����ʵ� �����Ѵ�.
	/// 
	/// 23.08.16 ������ ��.
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

	/// ���� �ε��Ѵ�.
	/// �ٷ� �������� �ʰ�, ���� �������� ������ �� �����Ѵ�.
	/// �����ϸ鼭 �ʱ�ȭ�� �����Ѵ�.
	/// 
	/// 23.08.16 ������ ��.
	bool SceneSystem::LoadScene(const std::string& sceneName)
	{
		auto iter = _sceneMap.find(sceneName);
		if (iter == _sceneMap.end())
		{
			// �������� �ʴ� ���Դϴ�.
			return false;
		}

		// ã�� ���� �ּҸ� �ѱ��.
		//_currentScene->Finalize();
		_loadScene = iter->second;
		return true;
	}

}