#pragma once
#include "Vec3.h"
#include <vector>
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "Colors.h"

class Pyramid
{
public:
	Pyramid(float size)
	{
		const float side = size;
		vertices.emplace_back( side, 0.0f,		 0.0f);
		vertices.emplace_back( 0.0f, 0.0f,		-side);
		vertices.emplace_back(-side, 0.0f,		 0.0f);
		vertices.emplace_back( 0.0f, 0.0f,		 side);
		vertices.emplace_back( 0.0f, 1.0f*side, 0.0f);
		Vec3 m0 = (vertices[0] + vertices[1] + vertices[4]) / 3;
		Vec3 n0 = (vertices[1] - vertices[0]) % (vertices[4] - vertices[0]).GetNormalized();
		//normals_axes.emplace_back(m0, m0+n0, Colors::Yellow); // Axis0
		normals_axes.emplace_back(m0, m0 + m0.GetNormalized(), Colors::Yellow); // Axis0

		Vec3 m1 = (vertices[1] + vertices[2] + vertices[4]) / 3;
		Vec3 n1 = (vertices[2] - vertices[1]) % (vertices[4] - vertices[1]).GetNormalized();
		//normals_axes.emplace_back(m1, m1 + n1, Colors::White); // Axis1
		normals_axes.emplace_back(m1, m1 + m1.GetNormalized(), Colors::White); // Axis1

		Vec3 m2 = (vertices[2] + vertices[3] + vertices[4]) / 3;
		Vec3 n2 = (vertices[3] - vertices[2]) % (vertices[4] - vertices[2]).GetNormalized();
		//normals_axes.emplace_back(m2, m2 + n2, Colors::White); // Axis2
		normals_axes.emplace_back(m2, m2 + m2.GetNormalized(), Colors::White); // Axis2

		Vec3 m3 = (vertices[3] + vertices[0] + vertices[4]) / 3;
		Vec3 n3 = (vertices[0] - vertices[3]) % (vertices[4] - vertices[3]).GetNormalized();
		//normals_axes.emplace_back(m3, m3 + n3, Colors::White); // Axis3
		normals_axes.emplace_back(m3, m3 + m3.GetNormalized(), Colors::White); // Axis3

		Vec3 m4 = {0,0,0};
		Vec3 n4 = Vec3(0,-1,0).GetNormalized()/2;
		normals_axes.emplace_back(m4, m4 + n4, Colors::White); // Axis4a
		//normals_axes.emplace_back(m4, m4 + n4, Colors::White); // Axis4b
		normals_axes.emplace_back(normals_axes.back());

		Vec3 m5 = vertices[4];
		Vec3 n5 = Vec3(0, 1, 0).GetNormalized() / 2;
		normals_axes.emplace_back(m5, m5 + n5, Colors::Green);  // Y-Axis
		
		Vec3 m6 = vertices[0];
		Vec3 n6 = Vec3(1, 0, 0).GetNormalized() / 2;
		normals_axes.emplace_back(m6, m6 + n6, Colors::Blue);  // X-Axis

		Vec3 m7 = vertices[3];
		Vec3 n7 = Vec3(0, 0, 1).GetNormalized() / 2;
		normals_axes.emplace_back(m7, m7 + n7, Colors::Red);  // Z-Axis

	}
	IndexedLineList GetLines() const
	{
		return { vertices, {
			0,1,1,4,4,0,
			1,2,2,4,4,1,
			2,4,4,3,3,2,
			3,4,4,0,0,3
			} };
	}
	IndexedTriangleList<Vec3> GetTriangles() const
	{
		return {
			vertices,
			{
			0,1,4,
			1,2,4,
			2,3,4,
			3,0,4,
			0,3,2,
			2,1,0
			},
			normals_axes
		};
	}
private:
	std::vector<Vec3> vertices;
	std::vector<Axis> normals_axes;
}; 
