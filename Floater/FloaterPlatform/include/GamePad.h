#pragma once


namespace flt
{
	enum class GamePadType
	{
		XBOX,
		UNKNOWN
	};

	struct GamePadDeadZone
	{
		float leftStick = 0.1f;
		float rightStick = 0.1f;
		float lTrigger = 0.1f;
		float rTrigger = 0.1f;
	};

	struct GamePadState
	{
		enum ButtonFlag : int
		{
			A =			0x00000001,
			B =			0x00000002,
			X =			0x00000004,
			Y =			0x00000008,
			LB =		0x00000010,
			RB =		0x00000020,
			LT =		0x00000040,
			RT =		0x00000080,
			BACK =		0x00000100,
			START =		0x00000200,
			LSTICK =	0x00000400,
			RSTICK =	0x00000800,
			UP =		0x00001000,
			RIGHT =		0x00002000,
			DOWN =		0x00004000,
			LEFT =		0x00008000,
			KEY1 =		0x00010000,
			KEY2 =		0x00020000,
			KEY3 =		0x00040000,
			KEY4 =		0x00080000,
			KEY5 =		0x00100000,
			KEY6 =		0x00200000,
			KEY7 =		0x00400000,
			KEY8 =		0x00800000
		};

		int buttons;
		int buttonsDown;
		int buttonsUp;
		float lStickX;
		float lStickY;
		float rStickX;
		float rStickY;
		float lTrigger;
		float rTrigger;
	};

	struct GamePadVibration
	{
		float leftMotorPower;
		float leftMotorTime;
		float rightMotorPower;
		float rightMotorTime;
	};
}
