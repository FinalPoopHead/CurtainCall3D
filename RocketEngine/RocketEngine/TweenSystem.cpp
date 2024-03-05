#include "TweenSystem.h"
#include "Tween.h"
#include "TweenTimer.h"

namespace Rocket::Core
{
	TweenSystem::TweenSystem()
	{

	}

	void TweenSystem::Initialize()
	{

	}

	void TweenSystem::Finalize()
	{

	}

	Rocket::Tween& TweenSystem::CreateTween()
	{
		Rocket::Tween* temp = new Rocket::Tween();
		_dotweens.push_back(temp);
		return *_dotweens.back();
	}


	void TweenSystem::Update()
	{

		for (Rocket::Tween* tween : _dotweens)
		{
			if (tween->_timer->_isActive == true)
			{
				tween->_timer->Update();
			}
			else
			{
				// 끝난 오브젝트를 모아두는 벡터에 현재 사용이 끝난 이터레이터를 넣는다
				if (tween->onCompleteFunc != nullptr)
				{
					tween->onCompleteFunc();
				}
				_deleteObj.push_back(tween);
			}
		}

		// 지울 오브젝트를 모아둔 벡터와 현재 벡터를 비교해서 지운다
		auto findObj = _deleteObj.begin();
		for (Rocket::Tween* i : _deleteObj)
		{
			auto dotweenIterator = std::find(_dotweens.begin(), _dotweens.end(), *findObj);
			if (dotweenIterator != _dotweens.end())
			{
				delete* dotweenIterator;
				_dotweens.erase(dotweenIterator);
			}
			++findObj;
		}

		// 지울 오브젝트를 모아둔 벡터를 클리어한다
		_deleteObj.clear();

	}
}
