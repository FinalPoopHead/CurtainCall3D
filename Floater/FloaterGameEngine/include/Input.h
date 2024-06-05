#pragma once
#include "../../FloaterPlatform/include/KeyCode.h"
#include "../../FloaterPlatform/include/GamePad.h"
#include <vector>

namespace flt
{
	KeyData GetKey(KeyCode code);

	bool GetGamePadState(int padIndex, GamePadState* outState);

	bool SetGamePadVibration(int padIndex, float leftMotor, float rightMotor);

	std::vector<int> GetGamePadIndexList();
}

