#pragma once
#include <limits>

class ZBuffer
{
public:
	ZBuffer(int width_in, int height_in)
		:
		width(width_in),
		height(height_in),
		zBuffer(new float[(size_t)width*height])
	{
	}
	~ZBuffer()
	{
		delete[] zBuffer;
		zBuffer = nullptr;
	}
	void Clear()
	{
		for (size_t i = 0; i < (size_t)width * height; i++)
		{
			zBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}
	bool TestAndSet(int x, int y, float z)
	{
		if (z < zBuffer[y * width + x])
		{
			zBuffer[y * width + x] = z;
			return true;
		}
		return false;
	}
private:
	int width;
	int height;
	float* zBuffer = nullptr;
};