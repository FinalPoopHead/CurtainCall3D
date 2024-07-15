#include "./include/Ease.h"
#include "./include/Bezier.h"
#include <cmath>


const flt::ease::EaseFunction flt::ease::linear = [](float t){return t;};
const flt::ease::EaseFunction flt::ease::ease = [](float t){return t * t * (3.0f - 2.0f * t);};

/// Ease In
const flt::ease::EaseFunction flt::ease::easeIn = [](float t){return t * t;};
const flt::ease::EaseFunction flt::ease::easeInSine = [](float t){return 1.0f - cosf(t * flt::PI<float> * 0.5f);};
const flt::ease::EaseFunction flt::ease::easeInCubic = [](float t){return t * t * t;};
const flt::ease::EaseFunction flt::ease::easeInQuint = [](float t){return t * t * t * t * t;};
const flt::ease::EaseFunction flt::ease::easeInCirc = [](float t){return 1.0f - sqrtf(1.0f - t * t);};
const flt::ease::EaseFunction flt::ease::easeInElastic = [](float t)
	{
		return -powf(2.0f, 10.0f * t - 1.0f) * sinf((10.0f * t - 10.75f) * 2.0f * flt::PI<float> / 3.0f);
	};
const flt::ease::EaseFunction flt::ease::easeInQuad = [](float t){return t * t;};
const flt::ease::EaseFunction flt::ease::easeInQuart = [](float t){return t * t * t * t;};
const flt::ease::EaseFunction flt::ease::easeInExpo = [](float t){return t == 0.0f ? 0.0f : powf(2.0f, 10.0f * (t - 1.0f));};
const flt::ease::EaseFunction flt::ease::easeInBack = [](float t){return t * t * (2.70158f * t - 1.70158f);};
const flt::ease::EaseFunction flt::ease::easeInBounce = [](float t){return 1.0f - flt::ease::easeOutBounce(1.0f - t);};

/// Ease Out
const flt::ease::EaseFunction flt::ease::easeOut = [](float t){return t * (2.0f - t);};
const flt::ease::EaseFunction flt::ease::easeOutSine = [](float t){return sinf(t * flt::PI<float> * 0.5f);};
const flt::ease::EaseFunction flt::ease::easeOutCubic = [](float t){return (--t) * t * t + 1.0f;};
const flt::ease::EaseFunction flt::ease::easeOutQuint = [](float t){return 1.0f + (--t) * t * t * t * t;};
const flt::ease::EaseFunction flt::ease::easeOutCirc = [](float t){return sqrtf(1.0f - (--t) * t);};
const flt::ease::EaseFunction flt::ease::easeOutElastic = [](float t)
	{
		return powf(2.0f, -10.0f * t) * sinf((10.0f * t - 0.75f) * 2.0f * flt::PI<float> / 3) + 1.0f;
	};
const flt::ease::EaseFunction flt::ease::easeOutQuad = [](float t){return t * (2.0f - t);};
const flt::ease::EaseFunction flt::ease::easeOutQuart = [](float t){return 1.0f - (--t) * t * t * t;};
const flt::ease::EaseFunction flt::ease::easeOutExpo = [](float t){return t == 1.0f ? 1.0f : 1.0f - powf(2.0f, -10.0f * t);};
const flt::ease::EaseFunction flt::ease::easeOutBack = [](float t){return (--t) * t * (2.70158f * t + 1.70158f) + 1.0f;};
const flt::ease::EaseFunction flt::ease::easeOutBounce = [](float t)
	{
		if (t < 1 / 2.75f) {
			return 7.5625f * t * t;
		}
		else if (t < 2 / 2.75f) {
			t -= 1.5f / 2.75f;
			return 7.5625f * t * t + 0.75f;
		}
		else if (t < 2.5 / 2.75) {
			t -= 2.25f / 2.75f;
			return 7.5625f * t * t + 0.9375f;
		}
		else {
			t -= 2.625f / 2.75f;
			return 7.5625f * t * t + 0.984375f;
		}
	};

/// Ease In Out
const flt::ease::EaseFunction flt::ease::easeInOut = [](float t){return t * t * (3.0f - 2.0f * t);};
const flt::ease::EaseFunction flt::ease::easeInOutSine = [](float t){return 0.5f * (1.0f - cosf(flt::PI<float> * t));};
const flt::ease::EaseFunction flt::ease::easeInOutCubic = [](float t){return t < 0.5f ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;};
const flt::ease::EaseFunction flt::ease::easeInOutQuint = [](float t){return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - powf(-2.0f * t + 2.0f, 5.0f) / 2.0f;};
const flt::ease::EaseFunction flt::ease::easeInOutCirc = [](float t){return t < 0.5f ? (1.0f - sqrtf(1.0f - t * t)) / 2.0f : (sqrtf(1.0f - (t * 2.0f - 2.0f) * (t * 2.0f - 2.0f)) + 1.0f) / 2.0f;};
const flt::ease::EaseFunction flt::ease::easeInOutElastic = [](float t) 
	{
		if (t < 0.5f)
		{
			//return 0.5f * sinf(13.0f * flt::PI<float> *0.5f * 2.0f * t) * powf(2.0f, 10.0f * (2.0f * t - 1.0f));
			return (-powf(2.0f, 20.0f * t - 10.0f) * sinf((20.0f * t - 11.125f) * 2.0f * flt::PI<float> / 4.5f)) * 0.5f;
		}
		
		//return 0.5f * (sinf(-13.0f * flt::PI<float> *0.5f * (2.0f * t - 1.0f)) * powf(2.0f, -10.0f * (2.0f * t - 1.0f)) + 2.0f);
		return powf(2.0f, -20.0f * t +10.0f) * sinf((20.0f * t - 11.125f) * 2.0f * flt::PI<float> / 4.5f) * 0.5f + 1.0f;
	};
const flt::ease::EaseFunction flt::ease::easeInOutQuad = [](float t){return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;};
const flt::ease::EaseFunction flt::ease::easeInOutQuart = [](float t){return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - powf(-2.0f * t + 2.0f, 4.0f) / 2.0f;};
const flt::ease::EaseFunction flt::ease::easeInOutExpo = [](float t) {return t == 0 ? 0 : t == 1 ? 1 : t < 0.5f ? powf(2.0f, 20.0f * t - 10) / 2.0f : (2 - powf(2.0f, -20.0f * t + 10)) / 2.0f; };
const flt::ease::EaseFunction flt::ease::easeInOutBack = [](float t) {return t < 0.5f ? 2.0f * t * t * (3.5949095f * t - 2.5949095f) : 1.0f + 2.0f * (--t) * t * (3.5949095f * t + 2.5949095f); };
const flt::ease::EaseFunction flt::ease::easeInOutBounce = [](float t) 
	{
		if (t < 0.5f)
		{
			return (1.0f - flt::ease::easeOutBounce(1.0f - 2.0f * t)) / 2.0f;
		}

		return (1.0f + flt::ease::easeOutBounce(2.0f * t - 1.0f)) / 2.0f; 
	};


const flt::ease::EaseFunction flt::ease::bezierEase = [](float t) { return flt::Bezier::Ease()(t); };
const flt::ease::EaseFunction flt::ease::bezierEaseIn = [](float t) { return flt::Bezier::EaseIn()(t); };
const flt::ease::EaseFunction flt::ease::bezierEaseOut = [](float t) { return flt::Bezier::EaseOut()(t); };
const flt::ease::EaseFunction flt::ease::bezierEaseInOut = [](float t) { return flt::Bezier::EaseInOut()(t); };
