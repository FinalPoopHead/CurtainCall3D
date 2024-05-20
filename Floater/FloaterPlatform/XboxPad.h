#pragma once
#include "WinGamePadImpl.h"
#include <windows.h>
#include <string>

namespace flt
{
	namespace Xbox
	{
		enum KeyFlag
		{
			XBOX_DPAD_UP         = 0x0001,
			XBOX_DPAD_DOWN       = 0x0002,
			XBOX_DPAD_LEFT       = 0x0004,
			XBOX_DPAD_RIGHT      = 0x0008,
			XBOX_START           = 0x0010,
			XBOX_BACK            = 0x0020,
			XBOX_LEFT_THUMB      = 0x0040,
			XBOX_RIGHT_THUMB     = 0x0080,
			XBOX_LEFT_SHOULDER   = 0x0100,
			XBOX_RIGHT_SHOULDER  = 0x0200,
			XBOX_GUIDE           = 0x0400,
			XBOX_A               = 0x1000,
			XBOX_B               = 0x2000,
			XBOX_X               = 0x4000,
			XBOX_Y               = 0x8000
		};

		struct State
		{
			DWORD packet;
			WORD buttons;
			BYTE lTrigger;
			BYTE rTrigger;
			SHORT lStickX;
			SHORT lStickY;
			SHORT rStickX;
			SHORT rStickY;
		};

		struct Caps
		{
			BYTE type;
			BYTE subType;
			BYTE flags;
			DWORD buttons;
			BYTE lTrigger;
			BYTE rTrigger;
			SHORT lStickX;
			SHORT lStickY;
			SHORT rStickX;
			SHORT rStickY;
			BYTE lowFreq;
			BYTE highFreq;
		};

		bool Initialize(WinGamePad** outGamePadArr);
		bool Connect(WinGamePad* outGamePad);
		bool Disconnect(WinGamePad* outGamePad);
		bool Get(WinGamePad* outGamePad);
		bool Clear(WinGamePad* outGamePad);
		bool Set(WinGamePad* outGamePad, BYTE lMotor, BYTE rMotor);

		constexpr GUID guid = { 0xec87f1e3, 0xc13b, 0x4100, { 0xb5, 0xf7, 0x8b, 0x84, 0xd5, 0x42, 0x60, 0xcb } };
	}
}
