#pragma once
#include "../../FloaterPlatform/include/KeyCode.h"
#include "../../FloaterPlatform/include/GamePad.h"

namespace flt
{
	KeyData GetKey(KeyCode code);

	bool GetGamePadState(int padIndex, GamePadState* outState);
}

