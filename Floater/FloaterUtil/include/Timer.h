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
		void Stop();
		void Update();

		double GetTotalSeconds();
		double GetLabTimeSeconds();
		int64 GetLabTimeMicroSeconds();
		double GetDeltaSeconds();
	private:
		std::chrono::time_point<std::chrono::steady_clock> _init;
		std::chrono::time_point<std::chrono::steady_clock> _start;
		int64 _deltaMicroSeconds;
	};
}
