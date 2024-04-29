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
}
