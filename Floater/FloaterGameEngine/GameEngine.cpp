#include "./include/internal/GameEngine.h"
#include "./include/internal/Scene.h"
#include "../FloaterPlatform/include/Platform.h"
#include "PhysicsEngine.h"
#include "SoundEngine.h"


flt::IRenderer* flt::GameEngine::GetRenderer()
{
	return _renderer;
}

flt::PhysicsEngine* flt::GameEngine::GetPhysicsEngine()
{
	return _physicsEngine.get();
}

flt::SoundEngine* flt::GameEngine::GetSoundEngine()
{
	return _soundEngine.get();
}

flt::Platform* flt::GameEngine::GetPlatform()
{
	return _platform.get();
}

void flt::GameEngine::ExitGame()
{
	_platform->Exit();
}

void flt::GameEngine::Initialize()
{
	bool isDebug = false;
#ifdef _DEBUG
	isDebug = true;
#endif
	_platform = std::make_unique<Platform>(isDebug);
	_platform->Initialize(1280, 720, L"name", L"");
	_renderer = _platform->CreateRenderer(RendererType::ROCKET_DX11);
	//_renderer = _platform->CreateRenderer(RendererType::DX11);
	_physicsEngine = std::make_unique<PhysicsEngine>();
	_physicsEngine->Initialize();

	_soundEngine = std::make_unique<SoundEngine>();
	_soundEngine->Initialize();

	_timer.Start();
}

bool flt::GameEngine::Update()
{
	if (_nextScene)
	{
		ChangeScene();
	}
	ASSERT(_currentScene, "Scene is not set");

	_timer.Update();
	float deltaSecond = (float)_timer.GetDeltaSeconds();
	bool closeWindow = _platform->Update();

	_currentScene->StartFrame();

	constexpr float fixedUpdateInterval = 0.02f;
	_fixedUpdateElapsedSecond += deltaSecond;
	// 너무 한번에 물리 루프를 돌지 않게 임시 루프
	// 추후 물리 루프 1회에 들어가는 시간을 계산해서 루프 횟수 등을 조절 할 생각.
	if (_fixedUpdateElapsedSecond > 0.07f)
	{
		_fixedUpdateElapsedSecond = 0.07f;
	}
	while (_fixedUpdateElapsedSecond > fixedUpdateInterval)
	{
		_currentScene->PrePhysicsUpdate();
		_physicsEngine->Update(fixedUpdateInterval, _currentScene->_collisionPairs);
		_currentScene->PostPhysicsUpdate();
		_fixedUpdateElapsedSecond -= fixedUpdateInterval;
	}


	_currentScene->Update(deltaSecond);

	_currentScene->PreRender();
	_renderer->Render(deltaSecond);
	_currentScene->PostRender();

	_currentScene->EndFrame();

	return closeWindow;
}

void flt::GameEngine::Finalize()
{
	if (_currentScene != nullptr)
	{
		_currentScene->Finalize();
	}
	_currentScene = nullptr;

	for (auto& [sceneName, scene] : _scenes)
	{
		delete scene;
	}

	_platform->DestroyRenderer(_renderer);
	//delete _platform;

	_renderer = nullptr;
	_platform = nullptr;

	_physicsEngine->Finalize();
	//delete _physicsEngine;
	_physicsEngine = nullptr;

	_soundEngine->Finalize();
	//delete _soundEngine;
	_soundEngine = nullptr;

	// 여기에서 해도 되는지 모르겠다.
	delete _instance;
	_instance = nullptr;
}

flt::Scene* flt::GameEngine::SetScene(Scene* scene)
{
	//ASSERT(_scenes.find(scene) != _scenes.end(), "Scene is not added");
	_nextScene = scene;
	Scene* ret = _currentScene;

	return ret;
}

flt::Scene* flt::GameEngine::SetScene(const std::wstring& sceneName)
{
	auto iter = _scenes.find(sceneName);
	ASSERT(iter != _scenes.end(), "Scene is not added");

	_nextScene = iter->second;
	Scene* ret = _currentScene;

	return ret;
}

bool flt::GameEngine::AddScene(const std::wstring& sceneName, Scene* scene)
{
	auto iter = _scenes.find(sceneName);
	if (iter != _scenes.end())
	{
		return false;
	}
	_scenes.insert({ sceneName, scene });
	return true;
}

flt::Scene* flt::GameEngine::GetCurrentScene()
{
	return _currentScene;
}

flt::Vector2f flt::GameEngine::GetWindowSize()
{
	return _platform->GetWindowSize();
}

flt::GameEngine::GameEngine() :
	_platform(nullptr),
	_renderer(nullptr),
	_physicsEngine(nullptr),
	_soundEngine(nullptr),
	_nextScene(nullptr),
	_currentScene(nullptr),
	_timer(),
	_fixedUpdateTimer(),
	_fixedUpdateElapsedSecond(0.0f)
{

}

flt::GameEngine::~GameEngine()
{

}

flt::GameEngine* flt::GameEngine::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new GameEngine();
		_instance->Initialize();
	}
	return _instance;
}

void flt::GameEngine::ChangeScene()
{
	ASSERT(_nextScene, "ChangeScene is nullptr");
	if (_currentScene != nullptr)
	{
		_currentScene->Finalize();
		_currentScene->EndScene();
	}

	_currentScene = _nextScene;
	_currentScene->StartScene();
	_currentScene->Initialize();

	_nextScene = nullptr;
}

flt::GameEngine* flt::GameEngine::_instance = nullptr;
