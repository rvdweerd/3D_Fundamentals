#pragma once
#include "Vec3.h"
#include "Vec2.h"

class TexVertex
{
public:
	TexVertex(const Vec3& pos, const Vec2& tc)
		:
		pos(pos),
		tc(tc)
	{}
	TexVertex InterpolateTo( const TexVertex& dest, float alpha) const
	{
		return {
			pos.InterpolateTo( dest.pos,alpha),
			tc.InterpolateTo ( dest.tc, alpha)
		};
	}
	TexVertex operator-(const TexVertex& rhs) const
	{
		return {
			pos - rhs.pos,
			tc - rhs.tc
		};
	}
	TexVertex operator/(const float k) const
	{
		return {
			pos / k,
			tc / k
		};
	}
	TexVertex operator*(const float k) const
	{
		return {
			pos * k,
			tc * k
		};
	}
	TexVertex& operator+=(const TexVertex& rhs)
	{
		pos += rhs.pos;
		tc += rhs.tc;
		return *this;
	}
	Vec3 pos;
	Vec2 tc;
};