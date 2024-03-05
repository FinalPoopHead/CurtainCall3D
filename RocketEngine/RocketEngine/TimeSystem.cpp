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

	/// TimeSystem 초기화
	/// 22.12.18 강석원 집
	void TimeSystem::Initialize()
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_startTick);	// 초기 시작 값
	}

	void TimeSystem::Finalize()
	{

	}

	/// 한 프레임에 걸린 시간 측정 및 계산
	/// 22.12.18 강석원 집
	void TimeSystem::MeasureTime()
	{
		QueryPerformanceCounter(&_endTick);

		_elapsedTick = (float)(_endTick.QuadPart - _startTick.QuadPart) /
			(float)(_frequency.QuadPart);
		_deltaTime = _elapsedTick;// *1000;	// 1000을 곱하면 ms 단위 안곱하면 s단위

		AccumulateFrame(_deltaTime);

		QueryPerformanceCounter(&_startTick);
	}
}
