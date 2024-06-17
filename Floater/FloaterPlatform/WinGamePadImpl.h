#pragma once
#include "../FloaterUtil/include/FloaterType.h"
#include "./include/GamePad.h"
#include <string>
#include <windows.h>


namespace flt
{
	struct WinGamePad
	{
		WinGamePad() :
			path(),
			hash(0),
			handle(NULL),
			type(GamePadType::UNKNOWN),
			state(),
			vibration(),
			deadZone(),
			isConnected(false) {}

		std::wstring path;
		uint64 hash;
		HANDLE handle;
		GamePadType type;
		GamePadState state;
		GamePadVibration vibration;
		GamePadDeadZone deadZone;
		bool isConnected;
	};
}
