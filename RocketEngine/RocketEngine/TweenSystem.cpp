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
				// ���� ������Ʈ�� ��Ƶδ� ���Ϳ� ���� ����� ���� ���ͷ����͸� �ִ´�
				if (tween->onCompleteFunc != nullptr)
				{
					tween->onCompleteFunc();
				}
				_deleteObj.push_back(tween);
			}
		}

		// ���� ������Ʈ�� ��Ƶ� ���Ϳ� ���� ���͸� ���ؼ� �����
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

		// ���� ������Ʈ�� ��Ƶ� ���͸� Ŭ�����Ѵ�
		_deleteObj.clear();

	}
}
