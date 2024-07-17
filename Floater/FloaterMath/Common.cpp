#include <cmath>
#include "./include/Common.h"
#include <limits>

namespace flt
{
	float DegToRad(float degree)
	{
		return std::fmod(degree, 360.0f) * PI<float> / 180.f;
	}

	float RadToDeg(float radian)
	{
		return std::fmod(radian, 2.f * PI<float>) * 180.f / PI<float>;
	}

	//float FLOAT_EPSILON = 1.192092896e-07F; 
	// 1.0f == 1.0 + FLOAT_EPSILON 이 false가 나오는 가장 작은 값.
	float FLOAT_EPSILON = std::numeric_limits<float>::epsilon();

	float SmoothStep(float t)
	{
		if (t == 0.0f)
		{
			return 0.0f;
		}
		else if (t == 1.0f)
		{
			return 1.0f;
		}
		else
		{
			return t * t * (3.0f - 2.0f * t);
		}
	}

	float MoreSmoothStep(float t)
	{
		if (t == 0.0f)
		{
			return 0.0f;
		}
		else if (t == 1.0f)
		{
			return 1.0f;
		}
		else
		{
			return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
		}
	}
