#pragma once
#include <functional>

namespace flt
{
	namespace Ease
	{
		extern const std::function<float(float)> bezierLinear;
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
