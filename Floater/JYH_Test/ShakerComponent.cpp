#include "ShakerComponent.h"
#include "../FloaterMath/include/Ease.h"
#include <random>

Shaker::Shaker()
	: _doShake(false)
	, _duration(0.0f), _elapsed(0.0f)
	, _diffX(0.0f), _diffY(0.0f), _diffRad(0.0f)
	, _power(0.0f)
{
}

void Shaker::Impack(float power /*= 1.0f*/, float duration /*= 0.5f*/)
{
	_power = power;
	_duration = duration;
	_elapsed = 0.0f;

	_doShake = true;
}

void Shaker::DoIt(float amplitude, float frequency, float duration)
{

}

void Shaker::PreUpdate(float deltaSecond)
{
	if (_doShake)
	{
		_elapsed += deltaSecond;
		if (_elapsed >= _duration)
		{
			_gameObject->transform.AddLocalPosition(-_diffX, -_diffY, 0.0f);
			//_gameObject->transform.AddLocalRotation({ 0.0f, 0.0f, 1.0f }, -_diffRad);

			_diffX = 0.0f;
			_diffY = 0.0f;
			_diffRad = 0.0f;
			_duration = 0.0f;

			_doShake = false;
			return;
		}

		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_real_distribution<float> dis(-_power, _power);

		float t = _elapsed / _duration;
		float move = ImpactFunc(t);
		float x = dis(gen);
		float y = dis(gen);
		float rad = move * _power;
		_diffX += x;
		_diffY += y;
		float currRad = rad - _diffRad;
		_diffRad = rad;
		_gameObject->transform.AddLocalPosition(x, y, 0.0f);
		_gameObject->transform.AddLocalRotation({ 0.0f, 0.0f, 1.0f }, currRad);
	}
}

float Shaker::ImpactFunc(float t)
{
	float shakeCount = 3.0f;
	float attenuation = 5.0f;
	float amplitude = 2.0f;
	return amplitude * powf(2.0f, -powf(2, attenuation * t)) * sinf(shakeCount * flt::PI<float> *sqrtf(t));
}
