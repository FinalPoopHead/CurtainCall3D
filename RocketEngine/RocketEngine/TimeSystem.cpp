#include "TimeSystem.h"

namespace Rocket::Core
{
	float TimeSystem::_deltaTime = 0;
	int TimeSystem::_frameRate = 0;

	TimeSystem::TimeSystem()
		:_frequency(LARGE_INTEGER()),
		_startTick(LARGE_INTEGER()),
		_endTick(LARGE_INTEGER()),
		_elapsedTick(0.0)
	{

	}

	void TimeSystem::AccumulateFrame(float deltaTime)
	{
		static double elapsedTime = 0.0;
		static int frameCount = 0;

		elapsedTime += deltaTime;

		if (elapsedTime >= 1)
		{
			_frameRate = frameCount;
			frameCount = 0;
			elapsedTime = 0;
		}

		++frameCount;
	}

	/// TimeSystem �ʱ�ȭ
	/// 22.12.18 ������ ��
	void TimeSystem::Initialize()
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_startTick);	// �ʱ� ���� ��
	}

	void TimeSystem::Finalize()
	{

	}

	/// �� �����ӿ� �ɸ� �ð� ���� �� ���
	/// 22.12.18 ������ ��
	void TimeSystem::MeasureTime()
	{
		QueryPerformanceCounter(&_endTick);

		_elapsedTick = (float)(_endTick.QuadPart - _startTick.QuadPart) /
			(float)(_frequency.QuadPart);
		_deltaTime = _elapsedTick;// *1000;	// 1000�� ���ϸ� ms ���� �Ȱ��ϸ� s����

		AccumulateFrame(_deltaTime);

		QueryPerformanceCounter(&_startTick);
	}
}
