#include "./include/GameEngine.h"
#include "./include/Scene.h"
#include "../FloaterPlatform/include/Platform.h"
#include "PhysicsEngine.h"

flt::IRenderer* flt::GameEngine::GetRenderer()
{
	return _renderer;
}

flt::PhysicsEngine* flt::GameEngine::GetPhysicsEngine()
{
	return _physicsEngine;
}

flt::Platform* flt::GameEngine::GetPlatform()
{
	return _platform;
}

void flt::GameEngine::Initialize()
{
	bool isDebug = false;
#ifdef _DEBUG
	isDebug = true;
#endif
	_platform = new Platform(isDebug);
	_platform->Initialize(1280, 720, L"name", L"");
	_renderer = _platform->CreateRenderer(RendererType::ROCKET_DX11);
	//_renderer = _platform->CreateRenderer(RendererType::DX11);
	_physicsEngine = new PhysicsEngine();
	_physicsEngine->Initialize();

	_timer.Start();
}

bool flt::GameEngine::Update()
{
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
		_physicsEngine->Update(fixedUpdateInterval);
		_currentScene->PostPhysicsUpdate();
		_fixedUpdateElapsedSecond -= fixedUpdateInterval;
	}


	_currentScene->Update(deltaSecond);

	_renderer->Render(deltaSecond);
	_currentScene->EndRender();

	return closeWindow;
}

void flt::GameEngine::Finalize()
{
	_platform->DestroyRenderer(_renderer);
	delete _platform;

	_renderer = nullptr;
	_platform = nullptr;

	_physicsEngine->Finalize();
	delete _physicsEngine;
	_physicsEngine = nullptr;
}

void flt::GameEngine::SetScene(Scene* scene)
{
	if(_currentScene != nullptr)
	{
		_currentScene->Finalize();
	}

	_currentScene = scene;
	_currentScene->Initialize();
}

flt::GameEngine::GameEngine() : 
	_platform(nullptr), 
	_renderer(nullptr),
	_physicsEngine(nullptr),
	_currentScene(nullptr),
	_timer(),
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

flt::GameEngine* flt::GameEngine::_instance = nullptr;
