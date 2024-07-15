#include "./include/bezier.h"
#include "../FloaterUtil/include/FloaterMacro.h"

flt::Bezier flt::Bezier::Linear()
{
	return Bezier();
}

flt::Bezier flt::Bezier::Ease()
{
	Bezier bezier;
	bezier.AddControlPoint({0.25f, 0.1f});
	bezier.AddControlPoint({0.25f, 1.0f});

	return bezier;
}

flt::Bezier flt::Bezier::EaseIn()
{
	Bezier bezier;
	bezier.AddControlPoint({0.42f, 0.0f});

	return bezier;
}

flt::Bezier flt::Bezier::EaseOut()
{
	Bezier bezier;
	bezier.AddControlPoint({0.58f, 1.0f});

	return bezier;
}

flt::Bezier flt::Bezier::EaseInOut()
{
	Bezier bezier;
	bezier.AddControlPoint({0.42f, 0.0f});
	bezier.AddControlPoint({0.58f, 1.0f});

	return bezier;
}

flt::Bezier::Bezier() 
	: _controlPoints { {0.0f, 0.0f}, {1.0f, 1.0f} }
{

}

flt::Vector2f flt::Bezier::Evaluate(float t) const
{
	Vector2f result{0.0f, 0.0f};
	int size = (int)_controlPoints.size();
	int n = size - 1;
	for (int i = 0; i < size; i++)
	{
		float binomial = binomialCoefficient(n, i);
		result.x += binomial * std::powf(1.0f - t, (float)(n - i)) * std::powf(t, (float)i) * _controlPoints[i].x;
		result.y += binomial * std::powf(1.0f - t, (float)(n - i)) * std::powf(t, (float)i) * _controlPoints[i].y;
	}

	return result;
}

void flt::Bezier::AddControlPoint(const Vector2f& controlPoint)
{
	_controlPoints.back() = controlPoint;
	_controlPoints.push_back({1.0f, 1.0f});
}

float flt::Bezier::operator()(float x) const
{
	//float t = 0.5f;
	//constexpr int maxIterations = 100;
	//for (int i = 0; i < maxIterations; ++i)
	//{
	//	float calcY = CalcY(t);
	//	float calcYPrime = CalcYPrime(t);
	//	float nextT = t - (calcY - y) / calcYPrime;

	//	//float epsilon = std::max(std::fabsf(nextT), std::fabsf(t)) * flt::FLOAT_EPSILON;
	//	float epsilon = flt::FLOAT_EPSILON;
	//	if(std::fabsf(nextT - t) < epsilon)
	//	{
	//		t = nextT;
	//		break;
	//	}
	//	t = nextT;
	//}

	//return CalcX(t);
	float t = 0.5f;
	constexpr int maxIterations = 100;
	for (int i = 0; i < maxIterations; ++i)
	{
		float calcX = CalcX(t);
		float calcXPrime = CalcXPrime(t);
		float nextT = t - (calcX - x) / calcXPrime;

		float epsilon = flt::FLOAT_EPSILON;
		if (std::fabsf(nextT - t) < epsilon)
		{
			t = nextT;
			break;
		}
		t = nextT;
	}

	return CalcY(t);
}

std::array<std::array<float, 16>, 16> flt::Bezier::s_binomialCoefficients = std::array<std::array<float, 16>, 16>{std::array<float, 16>{0.0f} };

float flt::Bezier::binomialCoefficient(int n, int k)
{
	if (s_binomialCoefficients[n][k] != 0.0f)
	{
		return s_binomialCoefficients[n][k];
	}

	if (k > n - k)
	{
		k = n - k;
	}

	s_binomialCoefficients[n][k] = 1.0f;
	for (int i = 0; i < k; i++)
	{
		s_binomialCoefficients[n][k] *= (n - i);
		s_binomialCoefficients[n][k] /= (i + 1);
	}

	return s_binomialCoefficients[n][k];
}

float flt::Bezier::CalcX(float t) const
{
	float x = 0.0;
	int size = (int)_controlPoints.size();
	int n = size - 1;
	for (int i = 0; i < size; ++i) {
		float binomial = binomialCoefficient(n, i);
		x += binomial * std::powf(1.0f - t, (float)(n - i)) * std::powf(t, (float)i) * _controlPoints[i].x;
	}

	return x;
}

float flt::Bezier::CalcXPrime(float t) const
{
	float xPrime = 0.0f;
	int n = (int)_controlPoints.size() - 1;
	for (int i = 0; i < n; ++i)
	{
		float term = _controlPoints[i + 1].x - _controlPoints[i].x;
		xPrime += binomialCoefficient(n - 1, i) * std::powf(1.0f - t, (float)(n - i - 1)) * std::powf(t, (float)i) * term;
	}

	return xPrime * (float)n;
}

float flt::Bezier::CalcY(float t) const
{
	float y = 0.0f;
	int size = (int)_controlPoints.size();
	int n = size - 1;
	for (int i = 0; i < size; i++)
	{
		float binomial = binomialCoefficient(n, i);
		y += binomial * std::powf(1.0f - t, (float)(n - i)) * std::powf(t, (float)i) * _controlPoints[i].y;
	}

	return y;
}

float flt::Bezier::CalcYPrime(float t) const
{
	float yPrime = 0.0;
	int n = (int)_controlPoints.size() - 1;
	for(int i = 0; i < n; ++i)
	{
		float term = _controlPoints[i+1].y - _controlPoints[i].y;
		yPrime += binomialCoefficient(n - 1, i) * std::powf(1.0f - t, (float)(n - i - 1)) * std::powf(t, (float)i) * term;
	}

	return yPrime * (float)n;
}
