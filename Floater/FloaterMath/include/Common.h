#pragma once

#ifdef FLOATERMATH_EXPORTS
#define FLOATER_MATH_API __declspec(dllexport)
#else
#define FLOATER_MATH_API __declspec(dllimport)
#endif

namespace flt
{
	template<typename T>
	constexpr T PI = T(3.141592653589793238462643383279502884197169399375105820974944592307816406286);

	FLOATER_MATH_API float DegToRad(float degree);

	FLOATER_MATH_API float RadToDeg(float radian);

	FLOATER_MATH_API extern float FLOAT_EPSILON;
}
