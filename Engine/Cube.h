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
		const float side = size;// / 2.0f;
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
			1,5,4,  1,0,4, // top
			5,7,3,  5,1,3, // front
			7,5,4,  7,6,4, // left
			1,0,2,  1,3,2, // right
			0,4,6,  0,2,6, // back
			6,7,3,  6,2,3 // bottom
			} };
	}
private:
	std::vector<Vec3> vertices;

};
