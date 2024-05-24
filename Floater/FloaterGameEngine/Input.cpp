#include "./include/Input.h"
#include "./include/GameEngine.h"
#include "../FloaterPlatform/include/Platform.h"


static flt::Platform* platform = flt::GameEngine::Instance()->GetPlatform();

flt::KeyData flt::GetKey(KeyCode code)
{
	return platform->GetKey(code);  
}

bool flt::GetGamePadState(int padIndex, GamePadState* outState)
{
	return platform->GetGamePadState(padIndex, outState);
}

