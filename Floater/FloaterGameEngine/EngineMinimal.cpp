#include "./include/EngineMinimal.h"
#include "./include/internal/GameEngine.h"

namespace flt::__impl
{
	GameEngineWrapper g_engineWrapper;
}

static flt::GameEngine* g_engine = flt::GameEngine::Instance();

flt::GameEngineWrapper::GameEngineWrapper() : engine(GameEngine::Instance())
{
	//std::cout << "wrapper " << this << std::endl;
}

flt::Scene* flt::GameEngineWrapper::GetCurrentScene()
{
	return engine->GetCurrentScene();
}

bool flt::GameEngineWrapper::AddScene(const std::wstring& sceneName, flt::Scene* scene)
{
	return engine->AddScene(sceneName,scene);
}

flt::Scene* flt::GameEngineWrapper::GetScene(const std::wstring& sceneName)
{
	return engine->GetScene(sceneName);
}

flt::Scene* flt::GameEngineWrapper::SetScene(Scene* scene)
{
	return engine->SetScene(scene);
}

flt::Scene* flt::SetScene(Scene* scene)
{
	//GameEngine* engine = GameEngine::Instance();
	return g_engine->SetScene(scene);
}

flt::Vector2f flt::GetWindowSize()
{
	//GameEngine* engine = GameEngine::Instance();
	return g_engine->GetWindowSize();
}

flt::Scene* flt::SetScene(const std::wstring& sceneName)
{
	//GameEngine* engine = GameEngine::Instance();
	return g_engine->SetScene(sceneName);
}



void flt::ExitGame()
{
	//GameEngine* engine = GameEngine::Instance();
	g_engine->ExitGame();
}
