﻿#pragma once
#include "../../FloaterUtil/include/FloaterType.h"
#include "../../FloaterMath/include/floaterMath.h"
#include <string>


namespace flt
{
	enum class eTextAlignment
	{
		LEFT
		, CENTER
		, RIGHT
	};

	struct Text
	{
		Text()
			: data()
			, font()
			, color(0.0f, 0.0f, 0.0f, 1.0f)
			, size(0)
			, textAlign(eTextAlignment::LEFT)
		{}

		std::wstring data;
		std::wstring font;
		Vector4f color;
		uint32 size;
		eTextAlignment textAlign;
	};
}
