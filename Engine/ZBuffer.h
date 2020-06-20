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
	std::pair<bool,bool> TestAndSet(int x, int y, float z)
	{
		if (z < zBuffer[y * width + x])
		{
			if (abs(z - zBuffer[y * width + x]) < 0.01f)
			{
				zBuffer[y * width + x] = z;
				return { true,true };
			}
			else
			{
				zBuffer[y * width + x] = z;
				return { true,false };
			}
		}
		return { false,false };
	}
	std::pair<bool, bool> Test(int x, int y, float z)
	{
		if (z < zBuffer[y * width + x])
		{
			if (abs(z - zBuffer[y * width + x]) < 0.005f)
			{
				
				return { true,true };
			}
			else
			{
				
				return { true,false };
			}
		}
		return { false,false };
	}

private:
	int width;
	int height;
	float* zBuffer = nullptr;
};