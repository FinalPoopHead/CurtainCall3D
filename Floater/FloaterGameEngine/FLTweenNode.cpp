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

flt::FLTweenNode::FLTweenNode() : FLTweenNode(nullptr, 0.0f)
{

}

flt::FLTweenNode::FLTweenNode(std::function<float(float)> ease, float duration) :
	_ease(ease), _target(nullptr), _delay(0.0f), _duration(duration), _elapsed(0.0f), _from(0.0f), _to(0.0f)
	, _isFinished(false)
	, _onComplete(), _onStart(), _onPlay(), _onUpdate(), _onPause()
	, _next(nullptr)
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

void flt::FLTweenNode::SetEase(std::function<float(float)> ease)
{
	_ease = ease;
}

void flt::FLTweenNode::SetDuration(float duration)
{
	_duration = duration;
}
