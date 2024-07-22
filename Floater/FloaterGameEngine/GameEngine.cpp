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
	bool ret = _platform->Initialize(1920, 1080, L"", L"");
	ASSERT(ret, "Platform Initialize failed");
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
	bool isRun = true;
	if (_nextScene)
	{
		ChangeScene();
	}
	else
	{
		isRun = UpdateImpl(_currentScene);
	}

	return isRun;
}

void flt::GameEngine::Finalize()
{
	if (_currentScene != nullptr)
	{
		//_currentScene->OnDisable();
	}
	_currentScene = nullptr;

	for (auto& [sceneName, scene] : _scenes)
	{
		scene->Finalize();
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
	delete s_instance;
	s_instance = nullptr;
}

flt::Scene* flt::GameEngine::GetScene(const std::wstring& sceneName)
{
	auto iter = _scenes.find(sceneName);
	ASSERT(iter != _scenes.end(), "Scene is not added");

	return iter->second;
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

	return SetScene(iter->second);
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

void flt::GameEngine::SetWindowTitle(const std::wstring& title)
{
	_platform->SetWindowTitle(title);
}

flt::GameEngine::GameEngine() :
	_platform(nullptr),
	_renderer(nullptr),
	_physicsEngine(nullptr),
	_soundEngine(nullptr),
	_loadingScene(nullptr),
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
	if (s_instance == nullptr)
	{
		s_instance = new GameEngine();
		s_instance->Initialize();
	}
	return s_instance;
}

void flt::GameEngine::ChangeScene()
{
	ASSERT(_nextScene, "ChangeScene is nullptr");
	if (_currentScene != nullptr)
	{
		_currentScene->Finalize();
		_currentScene->EndScene();
		_currentScene->_isActive = false;
		//_currentScene->OnDisable();
	}

	_currentScene = _nextScene;
	_currentScene->_isActive = true;
	_currentScene->Initialize();
	_currentScene->StartScene();

	//UpdateImpl(_loadingScene);

	_nextScene = nullptr;
}

bool flt::GameEngine::UpdateImpl(Scene* scene)
{
	ASSERT(scene, "Scene is not set");

	_timer.Update();
	float deltaSecond = (float)_timer.GetDeltaSeconds();
	bool isOnWindows = _platform->Update(deltaSecond);
	_soundEngine->Update();

	scene->StartFrame();

	constexpr float fixedUpdateInterval = 0.02f;
	_fixedUpdateElapsedSecond += deltaSecond;
	// 너무 한번에 물리 루프를 돌지 않게 제한
	// 추후 물리 루프 1회에 들어가는 시간을 계산해서 루프 횟수 등을 조절 할 생각.
	if (_fixedUpdateElapsedSecond > 0.07f)
	{
		_fixedUpdateElapsedSecond = 0.07f;
	}
	while (_fixedUpdateElapsedSecond > fixedUpdateInterval)
	{
		scene->PrePhysicsUpdate();
		_physicsEngine->Update(fixedUpdateInterval, scene->_collisionPairs);
		scene->PostPhysicsUpdate();
		_fixedUpdateElapsedSecond -= fixedUpdateInterval;
	}


	scene->Update(deltaSecond);

	scene->PreRender();
	_renderer->Render(deltaSecond);
	scene->PostRender();

	scene->EndFrame();

	return isOnWindows;
}

flt::GameEngine* flt::GameEngine::s_instance = nullptr;
