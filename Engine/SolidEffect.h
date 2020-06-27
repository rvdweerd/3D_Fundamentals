#pragma once

#include "Pipeline.h"
#include "DefaultVertexShader.h"

class SolidEffect
{
public:
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos, const Color col)
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
				dest.color
			};
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return {
				pos - rhs.pos,
				color
			};
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return {
				pos + rhs.pos,
				color
			};
		}
		Vertex operator/(const float k) const
		{
			return {
				pos / k,
				color
			};
		}
		Vertex operator*(const float k) const
		{
			return {
				pos * k,
				color
			};
		}
		Vertex& operator*=(const float rhs)
		{
			pos *= rhs;
			//color *= rhs;
			return *this;
		}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			return *this;
		}
		Vec3 pos;
		Color color;
	};
	// Default VS: rotates and translates vertices (attributes untouched)
	typedef DefaultVertexShader<Vertex> VertexShader;

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
	VertexShader vs;
	PixelShader ps;
};
