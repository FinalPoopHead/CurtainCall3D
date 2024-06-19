#include "FLTween.h"
#include "FLTweenNode.h"


void flt::FLTween::Update(float deltaSeconds)
{
	if (_current)
	{
		_current->Update(deltaSeconds);

		if(_current->IsFinished())
		{
			_current = _current->Next();
		}
	}
}
