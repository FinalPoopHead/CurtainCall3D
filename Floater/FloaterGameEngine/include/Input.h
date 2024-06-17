#pragma once
#include "../../FloaterPlatform/include/KeyCode.h"
#include "../../FloaterPlatform/include/GamePad.h"
#include <vector>

namespace flt
{
	KeyData GetKey(KeyCode code);
	KeyData GetKeyDown(KeyCode code);
	KeyData GetKeyUp(KeyCode code);

	bool GetGamePadState(int padIndex, GamePadState* outState);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="padIndex"></param>
	/// <param name="isRightMotor">오른쪽 모터는 진동이 약한 모터, 왼쪽 모터는 진동이 강한 모터</param>
	/// <param name="moterPower">0~1 사이 값</param>
	/// <param name="time"></param>
	/// <returns>적용이 되면 TRUE</returns>
	bool SetGamePadVibration(int padIndex, bool isRightMotor, float moterPower, float time);

	std::vector<int> GetGamePadIndexList();
}

