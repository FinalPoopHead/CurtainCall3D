#pragma once
#include "FloaterMathDefine.h"

/// <summary>
/// https://easings.net/ko
/// 위 링크에서 참고했습니다.
/// </summary>

namespace flt
{
	namespace ease
	{
		using EaseFunction = float(*)(float);

		FLOATER_MATH_API extern const EaseFunction linear;
		FLOATER_MATH_API extern const EaseFunction ease;

		/// Ease In
		FLOATER_MATH_API extern const EaseFunction easeIn;
		FLOATER_MATH_API extern const EaseFunction easeInSine;
		FLOATER_MATH_API extern const EaseFunction easeInCubic;
		FLOATER_MATH_API extern const EaseFunction easeInQuint;
		FLOATER_MATH_API extern const EaseFunction easeInCirc;
		FLOATER_MATH_API extern const EaseFunction easeInElastic;
		FLOATER_MATH_API extern const EaseFunction easeInQuad;
		FLOATER_MATH_API extern const EaseFunction easeInQuart;
		FLOATER_MATH_API extern const EaseFunction easeInExpo;
		FLOATER_MATH_API extern const EaseFunction easeInBack;
		FLOATER_MATH_API extern const EaseFunction easeInBounce;
		
		/// Ease Out
		FLOATER_MATH_API extern const EaseFunction easeOut;
		FLOATER_MATH_API extern const EaseFunction easeOutSine;
		FLOATER_MATH_API extern const EaseFunction easeOutCubic;
		FLOATER_MATH_API extern const EaseFunction easeOutQuint;
		FLOATER_MATH_API extern const EaseFunction easeOutCirc;
		FLOATER_MATH_API extern const EaseFunction easeOutElastic;
		FLOATER_MATH_API extern const EaseFunction easeOutQuad;
		FLOATER_MATH_API extern const EaseFunction easeOutQuart;
		FLOATER_MATH_API extern const EaseFunction easeOutExpo;
		FLOATER_MATH_API extern const EaseFunction easeOutBack;
		FLOATER_MATH_API extern const EaseFunction easeOutBounce;

		/// Ease In Out
		FLOATER_MATH_API extern const EaseFunction easeInOut;
		FLOATER_MATH_API extern const EaseFunction easeInOutSine;
		FLOATER_MATH_API extern const EaseFunction easeInOutCubic;
		FLOATER_MATH_API extern const EaseFunction easeInOutQuint;
		FLOATER_MATH_API extern const EaseFunction easeInOutCirc;
		FLOATER_MATH_API extern const EaseFunction easeInOutElastic;
		FLOATER_MATH_API extern const EaseFunction easeInOutQuad;
		FLOATER_MATH_API extern const EaseFunction easeInOutQuart;
		FLOATER_MATH_API extern const EaseFunction easeInOutExpo;
		FLOATER_MATH_API extern const EaseFunction easeInOutBack;
		FLOATER_MATH_API extern const EaseFunction easeInOutBounce;

		/// 베지어 기반 이징. 현재 불완전한 상태
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
