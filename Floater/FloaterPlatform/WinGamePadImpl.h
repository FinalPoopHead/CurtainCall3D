#pragma once
#include "./include/GamePad.h"
#include <string>
#include <windows.h>


namespace flt
{
	struct WinGamePad
	{
		std::wstring path;
		HANDLE handle;
		GamePadType type;
		GamePadState state;
	};
}
