#include "./include/internal/MakeTween.h"
#include "./include/internal/GameEngine.h"


namespace
{
	flt::GameEngine* g_engine = flt::GameEngine::Instance();
}

void flt::internal::RegistTween(IFLTween* tween)
{
	g_engine->GetCurrentScene()->AddTween(tween);
}
