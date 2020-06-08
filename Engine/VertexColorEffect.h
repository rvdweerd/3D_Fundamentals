#pragma once

#include "Pipeline.h"

class VertexColorEffect
{
public:
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos, const Vec3& col)
			:
			pos(pos),
			color(col)
		{}
		Vertex(const Vec3& pos, const Vertex& v)
			:
			pos(pos),
			color(v.color)
		{
		}
		Vertex InterpolateTo(const Vertex& dest, float alpha) const
		{
			return {
				pos.InterpolateTo(dest.pos,alpha),
				color.InterpolateTo(dest.color, alpha)
			};
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return {
				pos - rhs.pos,
				color - rhs.color
			};
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return {
				pos + rhs.pos,
				color + rhs.color
			};
		}
		Vertex operator/(const float k) const
		{
			return {
				pos / k,
				color / k
			};
		}
		Vertex operator*(const float k) const
		{
			return {
				pos * k,
				color * k
			};
		}
		Vertex& operator*=(const float rhs)
		{
			pos *= rhs;
			color *= rhs;
			return *this;
		}

		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			color += rhs.color;
			return *this;
		}
		Vec3 pos;
		Vec3 color;
	};
	class PixelShader
	{
	public:
		//template<class Input>
		//Color operator()(const Input& in) const
		Color operator()(const Vertex& in) const
		{
			return Color(in.color);
		}
	};
public:
	PixelShader ps;
};
