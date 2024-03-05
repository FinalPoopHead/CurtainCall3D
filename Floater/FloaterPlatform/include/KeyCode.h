#pragma once
#include "../FloaterUtil/include/FloaterType.h"


namespace flt
{
	/// <summary>
	/// GetKey의 리턴을 위한 구조체
	/// keyTime 키 입력 시간. 지원하지 않을 경우 0
	/// x -> 마우스의 경우 상대 혹은 절대 좌표 x, 휠의경우 회전 정도 (양수: 위로, 음수: 아래로)
	/// y -> 마우스의 경우 상대 혹은 절대 좌표 y, 휠의경우 회전 정도 (양수: 위로, 음수: 아래로)
	/// </summary>
	struct KeyData
	{
		KeyData() : keyTime(0x0000'0000'0000'0000), x(0), y(0) {}
		KeyData(int64 keyTime, int32 x, int32 y) : keyTime(keyTime), x(x), y(y) {}

		operator bool() const noexcept
		{
			// 최상위 1비트가 1이면 false, 아니면 true.
			// 즉, 0보다 크거나 같으면 true, 음수면 false.
			return keyTime >= 0;

			//return !(bool)(keyTime >> 63);
			//return (bool)((keyTime & 0x8000'0000'0000'0000) ^ 0x8000'0000'0000'0000);
		}

		int64 keyTime;
		int32 x;
		int32 y;
	};

	enum class KeyCode
	{
		none,
		mouseLButton,
		mouseRButton,
		mouseMButton,
		mouseMup,
		mouseMdown,
		mouseButton3,
		mouseButton4,


		backspace,
		tab,
		enter,
		lShift,
		rShift,
		lCtrl,
		lAlt,
		RCtrl,
		capslock,
		esc,
		spacebar,
		pageUp,
		pageDown,
		end,
		home,
		RAlt,
		up,
		down,
		left,
		right,
		printScreen,
		insert,
		Delete,
		key0 = '0',
		key1,
		key2,
		key3,
		key4,
		key5,
		key6,
		key7,
		key8,
		key9,
		keyMinus,
		keyEqual,
		a = 'A',
		b,
		c,
		d,
		e,
		f,
		g,
		h,
		i,
		j,
		k,
		l,
		m,
		n,
		o,
		p,
		q,
		r,
		s,
		t,
		u,
		v,
		w,
		x,
		y,
		z,
		num0,
		num1,
		num2,
		num3,
		num4,
		num5,
		num6,
		num7,
		num8,
		num9,
		f1,
		f2,
		f3,
		f4,
		f5,
		f6,
		f7,
		f8,
		f9,
		f10,
		f11,
		f12,

		gpadA,
		gpadB,
		gpadX,
		gpadY,
		gpadLStick,	// 아날로그 데이터
		gpadRStick,	// 아날로그 데이터
		gpadLStickClick,
		gpadRStickClick,
		gpadLT,		// 아날로그 데이터
		gpadRT,		// 아날로그 데이터
		gpadLB,
		gpadRB,
		gpadUp,
		gpadDown,
		gpadLeft,
		gpadRight,
		gpadStart,
		gpadBack,

		mouseWheelUp,
		mouseWheelDown,

		mouseRelativePos,
		mouseAbsolutePos,

		MAX
	};
}
