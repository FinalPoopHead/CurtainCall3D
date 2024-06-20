#include "FLTweenNode.h"


void flt::FLTweenNode::Update(float deltaSeconds)
{
	_elapsed += deltaSeconds;
	if (_elapsed >= _duration)
	{
		_elapsed = _duration;
		_isFinished = true;
	}
}

flt::FLTweenNode::FLTweenNode(std::function<float(float)> ease, float duration) : 
	_ease(ease), _delay(0), _duration(duration), _elapsed(0), _isFinished(false), _next(nullptr)
{

}

bool flt::FLTweenNode::IsFinished()
{
	return _isFinished;
}

flt::FLTweenNode* flt::FLTweenNode::Next()
{
	return _next;
}

void flt::FLTweenNode::Append(FLTweenNode* next)
{
	FLTweenNode* last = this;
	while (last->Next() != nullptr)
	{
		last = last->Next();
	}

	last->_next = next;
}

void flt::FLTweenNode::SetDelay(float delaySecond)
{
	_delay = delaySecond;
}

void flt::FLTweenNode::SetEase(std::function<float(float)> ease)
{
	_ease = ease;
}

void flt::FLTweenNode::SetDuration(float duration)
{
	_duration = duration;
}
