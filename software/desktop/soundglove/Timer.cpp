#include "Timer.h"

double Timer::performance_counter_frequency = 1;

void Timer::initialize()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	performance_counter_frequency = (double)frequency.QuadPart;
}

Timer::Timer()
:   start_(),
	stop_()
{
}

void Timer::start()
{
	QueryPerformanceCounter(&start_);
}

void Timer::stop()
{
	QueryPerformanceCounter(&stop_);
}

double Timer::duration() const
{
	const double start_d = (double)start_.QuadPart;
	const double stop_d  = (double)stop_.QuadPart;
	return (stop_d - start_d) / performance_counter_frequency;
}
