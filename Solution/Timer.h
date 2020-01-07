#pragma once
#include <windows.h>
#include <iostream>
class Timer
{
public:
	Timer() {};
	~Timer() {};
	void StartCounter();
	double GetCounter();
private:
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
};

inline void Timer::StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart);

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

inline double Timer::GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}
