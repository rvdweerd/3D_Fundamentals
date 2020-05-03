#pragma once
#include "Vec3.h"
#include <vector>
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"

class Cube
{
public:
	Cube(float size)
	{
		const float side = size / 2.0f;
		vertices.emplace_back(  side, side, side  );
		vertices.emplace_back(  side, side,-side  );
		vertices.emplace_back(  side,-side, side  );
		vertices.emplace_back(  side,-side,-side  );
		vertices.emplace_back( -side, side, side  );
		vertices.emplace_back( -side, side,-side  );
		vertices.emplace_back( -side,-side, side  );
		vertices.emplace_back( -side,-side,-side  );
	}
	IndexedLineList GetLines() const
	{
		return { vertices, {
			5,4, 4,0, 0,1, 1,5,
			7,6, 6,2, 2,3, 3,7,
			7,5, 6,4, 2,0, 3,1
			} };
	}
	IndextedTriangleList GetTriangles() const
	{
		return { vertices, {
			1,5,4,  1,4,0, // top
			5,7,3,  5,1,3, // front
			7,5,4,  4,6,7, // left
			0,1,2,  1,2,3, // right
			4,6,0,  0,6,2, // back
			6,7,3,  6,3,2 // bottom
			} };
	}
private:
	std::vector<Vec3> vertices;

};
