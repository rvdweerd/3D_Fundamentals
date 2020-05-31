#pragma once
#include <chrono>

using namespace std::chrono;
class FrameTimer
{
public:
	FrameTimer()
	{
		last = steady_clock::now();
	}
	float Mark()
	{
		const auto old = last;
		last = steady_clock::now();
		const duration<float> frameTime = last - old;

		timer += (float)frameTime.count();
		cycles += 1;
		if (timer > 1.0f)
		{
			avgFR = cycles / timer;
			timer = 0.0f;
			cycles = 0;
		}
		return frameTime.count();
	}
	float GetFR() const
	{
		return avgFR;
	}
private:
	std::chrono::steady_clock::time_point last;
	float timer = 0;
	size_t cycles = 0;
	float avgFR = 0;
};


