#include "./include/Timer.h"

using namespace std::chrono;

#define DIV_NANO (1000000000.0)
#define DIV_MICRO (1000000.0)
#define DIV_MILLI (1000.0)


flt::Timer::Timer() :
	_init(),
	_start(),
	_deltaMicroSeconds(0)
{
	_init = high_resolution_clock::now();
	_start = _init;
}

void flt::Timer::Start()
{
	_start = high_resolution_clock::now();
}

void flt::Timer::Stop()
{
	_deltaMicroSeconds = duration_cast<microseconds>(high_resolution_clock::now() - _start).count();
}

void flt::Timer::Update()
{
	auto now = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(now - _start).count();
	_start = now;
	_deltaMicroSeconds = duration;
}

double flt::Timer::GetTotalSeconds()
{
	auto now = high_resolution_clock::now();
	return (duration_cast<microseconds>(now - _init).count()) / DIV_MICRO;
}

double flt::Timer::GetLabTimeSeconds()
{
	auto now = high_resolution_clock::now();
	return (duration_cast<microseconds>(now - _start).count()) / DIV_MICRO;
}

int64 flt::Timer::GetLabTimeMicroSeconds()
{
	auto now = high_resolution_clock::now();
	return (duration_cast<microseconds>(now - _start).count());
}

double flt::Timer::GetDeltaSeconds()
{
	return _deltaMicroSeconds / DIV_MICRO;
}
