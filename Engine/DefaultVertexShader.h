#pragma once
#include "Mat3.h"
#include "Vec3.h"

template <class Vertex>
class DefaultVertexShader
{
public:
	typedef Vertex Output;
public:
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
	Output operator()(const Vertex& in) const
	{
		return { in.pos * rotation + translation, in };
	}
	Vec3 operator()(const Vec3& in) const
	{
		return { in * rotation + translation };
	}
private:
	Mat3 rotation;
	Vec3 translation;
};