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
		[[nodiscard]] __declspec(dllexport) static Bezier Linear();
		[[nodiscard]] __declspec(dllexport) static Bezier Ease();
		[[nodiscard]] __declspec(dllexport) static Bezier EaseIn();
		[[nodiscard]] __declspec(dllexport) static Bezier EaseOut();
		[[nodiscard]] __declspec(dllexport) static Bezier EaseInOut();

	public:
		__declspec(dllexport) Bezier();
		__declspec(dllexport) ~Bezier() {}

		[[nodiscard]] __declspec(dllexport) Vector2f Evaluate(float t) const;
		[[nodiscard]] __declspec(dllexport) float operator()(float x) const;
		__declspec(dllexport) void AddControlPoint(const Vector2f& controlPoint);

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


