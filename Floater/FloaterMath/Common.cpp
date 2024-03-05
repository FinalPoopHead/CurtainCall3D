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
	float FLOAT_EPSILON = std::numeric_limits<float>::epsilon();
}
