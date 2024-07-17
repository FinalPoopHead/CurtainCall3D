#include "Shaker.h"
#include "../FloaterMath/include/Ease.h"

void Shaker::Impack(float power /*= 1.0f*/, float duration /*= 0.5f*/)
{
	_power = power;
	_duration = duration;
	_elapsed = 0.0f;
}

void Shaker::PreUpdate(float deltaSecond)
{
	if(_doShake)
	{
		_elapsed += deltaSecond;
		if(_elapsed >= _duration)
		{
			_gameObject->transform.AddLocalPosition(-_diffX, -_diffY, 0.0f);
			_gameObject->transform.AddLocalRotation({0.0f, 0.0f, 1.0f}, -_diffRad);

			_diffX = 0.0f;
			_diffY = 0.0f;
			_diffRad = 0.0f;
			_duration = 0.0f;

			_doShake = false;
		}

		float t = _elapsed / _duration;
		float _power = flt::ease::easeInOutElastic(t);
		_diffX += _power;
		_diffY += _power;
		_diffRad += _power;
		_gameObject->transform.AddLocalPosition(_power, _power, 0.0f);
		_gameObject->transform.AddLocalRotation({0.0f, 0.0f, 1.0f}, _power);
	}
}
