#include "./include/Input.h"
#include "./include/internal/GameEngine.h"
#include "../FloaterPlatform/include/Platform.h"

flt::KeyData flt::GetKey(KeyCode code)
{
	return flt::GameEngine::Instance()->GetPlatform()->GetKey(code);
}

flt::KeyData flt::GetKeyDown(KeyCode code)
{
	return flt::GameEngine::Instance()->GetPlatform()->GetKeyDown(code);
}

flt::KeyData flt::GetKeyUp(KeyCode code)
{
	return flt::GameEngine::Instance()->GetPlatform()->GetKeyUp(code);
}

bool flt::GetGamePadState(int padIndex, GamePadState* outState)
{
	return flt::GameEngine::Instance()->GetPlatform()->GetGamePadState(padIndex, outState);
}

bool flt::SetGamePadVibration(int padIndex, bool isRightMotor, float moterPower, float time)
{
	ASSERT(moterPower >= 0.0f && moterPower <= 1.0f, "moterPower 범위가 잘못되었습니다.");
	return flt::GameEngine::Instance()->GetPlatform()->SetGamePadVibration(padIndex, isRightMotor, moterPower, time);
}

std::vector<int> flt::GetGamePadIndexList()
{
	return flt::GameEngine::Instance()->GetPlatform()->GetGamePadIndexList();
}

