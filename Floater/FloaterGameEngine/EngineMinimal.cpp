#include "./include/EngineMinimal.h"
#include "./include/internal/GameEngine.h"

namespace flt::__impl
{
	GameEngineWrapper g_engine;
}


flt::GameEngineWrapper::GameEngineWrapper() : engine(GameEngine::Instance())
{
	std::cout << "wrapper " << this << std::endl;
}

flt::Scene* flt::GameEngineWrapper::GetCurrentScene()
{
	return engine->GetCurrentScene();
}

void flt::GameEngineWrapper::AddScene(flt::Scene* scene)
{
	return engine->AddScene(scene);
}

flt::Scene* flt::SetScene(Scene* scene)
{
	GameEngine* engine = GameEngine::Instance();
	return engine->SetScene(scene);
}
