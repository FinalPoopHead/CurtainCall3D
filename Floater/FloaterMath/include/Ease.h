#pragma once
#include "FloaterMathDefine.h"

namespace flt
{
	namespace ease
	{
		using EaseFunction = float(*)(float);

		FLOATER_MATH_API extern const EaseFunction linear;
		FLOATER_MATH_API extern const EaseFunction ease;
		FLOATER_MATH_API extern const EaseFunction easeIn;
		FLOATER_MATH_API extern const EaseFunction easeOut;
		FLOATER_MATH_API extern const EaseFunction easeInOut;

		FLOATER_MATH_API extern const EaseFunction bezierEase;
		FLOATER_MATH_API extern const EaseFunction bezierEaseIn;
		FLOATER_MATH_API extern const EaseFunction bezierEaseOut;
		FLOATER_MATH_API extern const EaseFunction bezierEaseInOut;
	}
	//class Ease
	//{
	//public:
	//	float operator()(float t) const
	//	{
	//		return ease(t);
	//	}

	//private:
	//	float (*ease)(float t);
	//};

	//class Linear : public Ease
	//{
	//public:
	//	Linear() : Ease()
	//	{
	//		ease = [](float t) -> float
	//			{
	//			return t;
	//		};
	//	}
	//};
}
