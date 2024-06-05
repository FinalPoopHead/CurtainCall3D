#include "./include/Input.h"
#include "./include/internal/GameEngine.h"
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

bool flt::SetGamePadVibration(int padIndex, float leftMotor, float rightMotor)
{
	return platform->SetGamePadVibration(padIndex, leftMotor, rightMotor);
}

std::vector<int> flt::GetGamePadIndexList()
{
	return platform->GetGamePadIndexList();
}

