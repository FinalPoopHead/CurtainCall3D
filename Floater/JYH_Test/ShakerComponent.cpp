#include "ShakerComponent.h"
#include "../FloaterMath/include/Ease.h"
#include <random>

ShakeComponent::ShakeComponent()
	: _shakers()
{
}

void ShakeComponent::Impack(float power, float duration, flt::Vector2f dir, std::function<float(float)> shakeFunc)
{
	if (dir.Norm() == 0.0f)
	{
		dir = { 1.0f, 1.0f };
	}
	_shakers.EmplaceBack(power, duration, dir, shakeFunc);
}

void ShakeComponent::DoIt(float amplitude, float frequency, float duration)
{

}

void ShakeComponent::PreUpdate(float deltaSecond)
{
	for (uint32 denseIndex = 0; denseIndex < _shakers.Size();)
	{
		Shaker& shaker = _shakers.AtDense(denseIndex);
		if (shaker.Shake(deltaSecond, _gameObject->transform))
		{
			_shakers.EraseDense(denseIndex);
		}
		else
		{
			++denseIndex;
		}
	}
}

float ImpactFunc(float t)
{
	float shakeCount = 3.0f;
	float attenuation = 5.0f;
	float amplitude = 2.0f;
	return amplitude * powf(2.0f, -powf(2, attenuation * t)) * sinf(shakeCount * flt::PI<float> *sqrtf(t));
}

ShakeComponent::Shaker::Shaker(float power, float duration, flt::Vector2f dir, std::function<float(float)> shakeFunction)
	: _power(power)
	, _duration(duration)
	, _elapsed(0.0f)
	, _offset(0.0f)
	, _dir(dir.Normalized())
	, _shakeFunction(shakeFunction)
{

}

bool ShakeComponent::Shaker::Shake(float deltaSecond, flt::Transform& transform)
{
	_elapsed += deltaSecond;
	if (_elapsed < _duration)
	{
		float t = _elapsed / _duration;
		float currOffset = _shakeFunction(t);
		float moveOffset = currOffset - _offset;
		_offset = currOffset;
		flt::Vector3f moveTo = (flt::Vector3f)transform.Up() * _dir.y + (flt::Vector3f)transform.Right() * _dir.x;
		moveTo *= moveOffset * _power;
		transform.AddLocalPosition(moveTo);

		return false;
	}
	else
	{
		flt::Vector3f moveTo = (flt::Vector3f)transform.Up() * _dir.y + (flt::Vector3f)transform.Right() * _dir.x;
		moveTo *= _offset * _power;
		transform.AddLocalPosition(-moveTo);

		return true;
	}
}
