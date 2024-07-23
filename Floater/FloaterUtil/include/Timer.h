#pragma once
#include <chrono>
#include "FloaterType.h"

namespace flt
{
	class Timer
	{
	public:
		Timer();

		void Start();

		// Start를 바꾸지 않고 deltaTime을 갱신한다.
		void Stop();

		// deltaTime을 갱신하고 Start를 now로 갱신한다.
		void Update();

		// 타이머를 생성한 시간부터 현재까지의 시간을 반환한다.
		double GetTotalSeconds();

		// deltaTime을 갱신하지 않고 start부터 현재까지의 시간을 반환한다.
		double GetLabTimeSeconds();
		int64 GetLabTimeMicroSeconds();
		double GetDeltaSeconds();

	private:
		std::chrono::time_point<std::chrono::steady_clock> _init;
		std::chrono::time_point<std::chrono::steady_clock> _start;
		int64 _deltaMicroSeconds;
	};
}
