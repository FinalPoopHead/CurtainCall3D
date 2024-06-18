#include "FlTween.h"

void flt::FlTween::update(float deltaSeconds)
{
	_elapsed += deltaSeconds;
	if (_elapsed >= _duration)
	{
		_elapsed = _duration;
		_isFinished = true;
	}
}
