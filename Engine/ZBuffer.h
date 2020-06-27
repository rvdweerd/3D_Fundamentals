#pragma once
#include <limits>

class ZBuffer
{
public:
	ZBuffer(int width_in, int height_in)
		:
		width(width_in),
		height(height_in),
		pBuffer(new float[(size_t)width*height])
	{
	}
	~ZBuffer()
	{
		delete[] pBuffer;
		pBuffer = nullptr;
	}
	ZBuffer(const ZBuffer&) = delete;
	ZBuffer& operator=(const ZBuffer&) = delete;
	void Clear()
	{
		for (size_t i = 0; i < (size_t)width * height; i++)
		{
			pBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}
	float& At(int x, int y)
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);
		return pBuffer[x + y * width];
	}
	const float& At(int x, int y) const
	{
		return const_cast<ZBuffer*>(this)->At(x, y);
	}
	std::pair<bool,bool> TestAndSet(int x, int y, float depth)
	{
		float& depthInBuffer = At(x, y);
		if (depth <= depthInBuffer)
		{
			if (abs(depth - depthInBuffer) < 0.01f)
			{
				depthInBuffer = depth;
				return { true,true }; // point within threshold of intersection
			}
			else
			{
				depthInBuffer = depth;
				return { true,false }; // point outside threshold of intersection
			}
		}
		return { false,false };
	}
	std::pair<bool, bool> Test(int x, int y, float depth)
	{
		const float& depthInBuffer = At(x, y);
		if (depth <= depthInBuffer+0.01f)
		{
			return { true,true };
		}
		return { false,false };
	}

private:
	int width;
	int height;
	float* pBuffer = nullptr;
};