#include "./include/Input.h"
#include "./include/internal/GameEngine.h"
#include "../FloaterPlatform/include/Platform.h"


static flt::GameEngine* gs_gameEngine = flt::GameEngine::Instance();
static flt::Platform* gs_platform = flt::GameEngine::Instance()->GetPlatform();

flt::KeyData flt::GetKey(KeyCode code)
{
	return gs_platform->GetKey(code);  
}

flt::KeyData flt::GetKeyDown(KeyCode code)
{
	return gs_platform->GetKeyDown(code);
}

flt::KeyData flt::GetKeyUp(KeyCode code)
{
	return gs_platform->GetKeyUp(code);
}

bool flt::GetGamePadState(int padIndex, GamePadState* outState)
{
	return gs_platform->GetGamePadState(padIndex, outState);
}

bool flt::SetGamePadVibration(int padIndex, bool isRightMotor, float moterPower, float time)
{
	ASSERT(moterPower >= 0.0f && moterPower <= 1.0f, "moterPower 범위가 잘못되었습니다.");
	return gs_platform->SetGamePadVibration(padIndex, isRightMotor, moterPower, time);
}

std::vector<int> flt::GetGamePadIndexList()
{
	return gs_platform->GetGamePadIndexList();
}

