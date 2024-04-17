#include "./include/GameEngine.h"
#include "./include/Scene.h"
#include "../FloaterPlatform/include/Platform.h"

flt::IRenderer* flt::GameEngine::GetRenderer()
{
	return _renderer;
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

	_timer.Start();
}

void flt::GameEngine::Update()
{
	_timer.Update();

	_currentScene->StartFrame();

	_platform->Update();

	float deltaSeconde = (float)_timer.GetDeltaSeconds();
	_currentScene->Update(deltaSeconde);

	_renderer->Render(deltaSeconde);
	_currentScene->EndRender();
}

void flt::GameEngine::Finalize()
{
	_platform->DestroyRenderer(_renderer);
	delete _platform;

	_renderer = nullptr;
	_platform = nullptr;
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
	_currentScene(nullptr),
	_timer()
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
	}
	return _instance;
}

flt::GameEngine* flt::GameEngine::_instance = nullptr;
