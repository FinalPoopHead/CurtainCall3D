#pragma once
#include "./include/GamePad.h"
#include <string>
#include <windows.h>


namespace flt
{
	struct WinGamePad
	{
		WinGamePad() :
			path(),
			handle(NULL),
			type(GamePadType::UNKNOWN),
			state() {}
		std::wstring path;
		HANDLE handle;
		GamePadType type;
		GamePadState state;
	};
}
