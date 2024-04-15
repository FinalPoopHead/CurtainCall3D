#include "./include/GameEngine.h"
#include "./include/Scene.h"
#include "../FloaterPlatform/include/Platform.h"

flt::IRenderer* flt::GameEngine::GetRenderer()
{
	return _renderer;
}

void flt::GameEngine::Init()
{
	_timer.Start();
}

void flt::GameEngine::Update()
{
	_timer.Update();
	float deltaSeconde = _timer.GetDeltaSeconds();
	_currentScene->Update(deltaSeconde);
	_renderer->Render(deltaSeconde);
	_currentScene->EndRender();
	_currentScene->EndFrame();
}

flt::GameEngine::GameEngine() : _platform(nullptr), _renderer(nullptr)
{
	bool isDebug = false;
#ifdef _DEBUG
	isDebug = true;
#endif
	_platform = new Platform(isDebug);

	_renderer = _platform->CreateRenderer(RendererType::ROCKET_DX11);
}

flt::GameEngine::~GameEngine()
{
	_platform->DestroyRenderer(_renderer);
	delete _platform;
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
