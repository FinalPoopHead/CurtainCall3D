#include "./include/EngineMinimal.h"
#include "./include/internal/GameEngine.h"

namespace flt::__impl
{
	GameEngineWrapper g_engine;
}


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

flt::Scene* flt::SetScene(Scene* scene)
{
	GameEngine* engine = GameEngine::Instance();
	return engine->SetScene(scene);
}

flt::Scene* flt::SetScene(const std::wstring& sceneName)
{
	GameEngine* engine = GameEngine::Instance();
	return engine->SetScene(sceneName);
}

void flt::ExitGame()
{
	GameEngine* engine = GameEngine::Instance();
	engine->ExitGame();
}
