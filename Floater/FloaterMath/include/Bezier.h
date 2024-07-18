#pragma once
#include "Vector2f.h"
#include <array>
#include <vector>
#include <map>


namespace flt
{
	class  Bezier
	{
	public:
		[[nodiscard]] FLOATER_MATH_API static Bezier Linear();
		[[nodiscard]] FLOATER_MATH_API static Bezier Ease();
		[[nodiscard]] FLOATER_MATH_API static Bezier EaseIn();
		[[nodiscard]] FLOATER_MATH_API static Bezier EaseOut();
		[[nodiscard]] FLOATER_MATH_API static Bezier EaseInOut();

	public:
		FLOATER_MATH_API Bezier();
		FLOATER_MATH_API ~Bezier() {}

		[[nodiscard]] FLOATER_MATH_API Vector2f Evaluate(float t) const;
		[[nodiscard]] FLOATER_MATH_API float operator()(float x) const;
		FLOATER_MATH_API void AddControlPoint(const Vector2f& controlPoint);

	private:
		static std::array<std::array<float, 16>, 16> s_binomialCoefficients;
		static float binomialCoefficient(int n, int k);

	private:
		float CalcX(float t) const;
		float CalcXPrime(float t) const;
		float CalcY(float t) const;
		float CalcYPrime(float t) const;

		std::vector<Vector2f> _controlPoints;
	};

	//class QuadraticBezier : public Bezier
	//{
	//public:

	//private:
	//};

	//class CubicBezier : public Bezier
	//{
	//public:

	//private:
	//};
}


