#pragma once
#include "Vec3.h"
#include <vector>
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "TexVertex.h"

class Cube
{
public:
	Cube(float size)
	{
		const float texDim = 2.0f; // used for texture wrapping
		const float side = size / 2.0f;
		vertices.emplace_back(  side, side, side  );
		vertices.emplace_back(  side, side,-side  );
		vertices.emplace_back(  side,-side, side  );
		vertices.emplace_back(  side,-side,-side  );
		vertices.emplace_back( -side, side, side  );
		vertices.emplace_back( -side, side,-side  );
		vertices.emplace_back( -side,-side, side  );
		vertices.emplace_back( -side,-side,-side  );
		tc.emplace_back(0.0f,   0.0f);	//0
		tc.emplace_back(texDim, 0.0f);	//1
		tc.emplace_back(0.0f,   texDim);//2
		tc.emplace_back(texDim, texDim);//3
		tc.emplace_back(texDim, 0.0f);	//4
		tc.emplace_back(0.0f,   0.0f);	//5
		tc.emplace_back(texDim, texDim);//6
		tc.emplace_back(0.0f,   texDim);//7

		normals_axes.emplace_back( Vec3{  side,	 0,		 0	  }, Vec3{  side*2.5f,	 0,			 0			}, Colors::Blue  ); // X-axis
		normals_axes.emplace_back( Vec3{ -side,	 0,		 0	  }, Vec3{ -side*1.5f,	 0,			 0			}, Colors::LightGray ); // left
		normals_axes.emplace_back( Vec3{ 0,		 side,	 0	  }, Vec3{  0,			 side*2.5f,	 0			}, Colors::Green ); // Y-axis
		normals_axes.emplace_back( Vec3{ 0,		-side,	 0	  }, Vec3{  0,			-side*1.5f,	 0			}, Colors::LightGray ); // bottom
		normals_axes.emplace_back( Vec3{ 0,		 0,		 side }, Vec3{  0,			 0,			 side* 2.5f }, Colors::Red   ); // Z-axis
		normals_axes.emplace_back( Vec3{ 0,		 0,		-side }, Vec3{  0,			 0,			-side* 1.5f }, Colors::LightGray ); // front
	}
	IndexedLineList GetLines() const
	{
		return { vertices, {
			5,4, 4,0, 0,1, 1,5,
			7,6, 6,2, 2,3, 3,7,
			7,5, 6,4, 2,0, 3,1
			} };
	}
	IndexedTriangleList<Vec3> GetTriangles() const
	{
		return { 
			vertices, 
			{
			1,0,2,  2,3,1, // right (X-axis)
			4,5,7,  7,6,4, // left
			1,5,4,  4,0,1, // top (Y-axis)
			6,7,3,  3,2,6, // bottom
			0,4,6,  6,2,0, // back (Z-axis)
			3,7,5,  5,1,3, // front
			},
			normals_axes
		};
	}
	IndexedTriangleList<TexVertex> GetTrianglesTex() const
	{
		std::vector<TexVertex> tverts;
		tverts.reserve(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			tverts.emplace_back(vertices[i], tc[i]);
		}
		return {
			std::move(tverts),
			{
				1,0,2,  2,3,1, // right (X-axis)
				4,5,7,  7,6,4, // left
				1,5,4,  4,0,1, // top (Y-axis)
				6,7,3,  3,2,6, // bottom
				0,4,6,  6,2,0, // back (Z-axis)
				3,7,5,  5,1,3, // front
			},
			normals_axes
		};
	}
private:
	std::vector<Vec3> vertices;
	std::vector<Vec2> tc;
	std::vector<Axis> normals_axes;
};
