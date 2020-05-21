#pragma once

#include <vector>
#include "Vec3.h"

struct Axis
{
	Axis(Vec3 v1,Vec3 v2, Color c_in)
		:
		p0(v1),
		p1(v2),
		col(c_in)
	{}
	Vec3 p0;
	Vec3 p1;
	Color col = Colors::White;
};
template <class T>
struct IndexedTriangleList
{
	IndexedTriangleList(std::vector<T> verts_in, std::vector<size_t> ind_in, std::vector<Axis> axs_in)
		:
		vertices(std::move(verts_in)),
		indices(std::move(ind_in)),
		normals_axes(axs_in)
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}
	std::vector<T> vertices;
	std::vector<size_t> indices;
	std::vector<Axis> normals_axes;
};