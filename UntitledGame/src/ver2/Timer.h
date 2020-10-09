#pragma once
#include <chrono>
#include <iostream>


class Timer
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;

public:
	Timer();
	~Timer();

private:
	void Stop();
};

