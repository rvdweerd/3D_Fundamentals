#pragma once
#include "Vec3.h"
#include <vector>

#include "IndexedTriangleList.h"
//#include "TexVertex.h"

class Cube
{
public:
	template<class V>
	static IndexedTriangleList<V> GetSkinned(float size = 1.0f)
	{
		const float side = size / 2.0f;
		const auto ConvertTexCoord = [](float u, float v)
		{
			//return Vec2{ (u + 1.0f) / 3.0f,v / 4.0f }; // SkinSurface
			return Vec2{ u / 4.0f,v / 3.0f }; // SkinSurface
			//return { v.x / 4.0f , v.y / 3.0f }; // SurfaceTile
		};
		//auto applySkinSurface = [](Vec2 v)->Vec2 {return { v.x / 4.0f , v.y / 3.0f }; };
		//auto applySurfaceTiles = [](Vec2 v, float k = 1.0f)->Vec2 {return { (v.x) * k , (v.y) * k }; };


		std::vector<Vec3> vertices;
		std::vector<Vec2> tc;
		std::vector<Axis> normals_axes;

		const float texDim = 2.0f; // used for texture wrapping
		
		vertices.emplace_back(-side, -side, -side);	// #0
		vertices.emplace_back(+side, -side, -side);	// #1
		vertices.emplace_back(-side, +side, -side); // #2		
		vertices.emplace_back(+side, +side, -side);	// #3
		vertices.emplace_back(+side, -side, -side);	// #1
		vertices.emplace_back(-side, -side, -side);	// #0
		vertices.emplace_back(-side, +side, +side);	// #6
		vertices.emplace_back(+side, +side, +side);	// #7
		vertices.emplace_back(+side, -side, +side);	// #5
		vertices.emplace_back(-side, -side, +side);	// #4
		vertices.emplace_back(-side, +side, +side);	// #6
		vertices.emplace_back(-side, -side, +side);	// #4
		vertices.emplace_back(-side, +side, -side);	// #2
		vertices.emplace_back(-side, -side, -side);	// #0
		
		tc.emplace_back(ConvertTexCoord(0.0f, 0.0f));	// (0,0)
		tc.emplace_back(ConvertTexCoord(0.0f, 1.0f));	// (0,1)	
		tc.emplace_back(ConvertTexCoord(1.0f, 0.0f));	// (1,0)
		tc.emplace_back(ConvertTexCoord(1.0f, 1.0f));	// (1,1)
		tc.emplace_back(ConvertTexCoord(1.0f, 2.0f));	// (1,2)
		tc.emplace_back(ConvertTexCoord(1.0f, 3.0f));	// (1,3)
		tc.emplace_back(ConvertTexCoord(2.0f, 0.0f));	// (2,0)
		tc.emplace_back(ConvertTexCoord(2.0f, 1.0f));	// (2,1)
		tc.emplace_back(ConvertTexCoord(2.0f, 2.0f));	// (2,2)
		tc.emplace_back(ConvertTexCoord(2.0f, 3.0f));	// (2,3)
		tc.emplace_back(ConvertTexCoord(3.0f, 1.0f));	// (3,1)
		tc.emplace_back(ConvertTexCoord(3.0f, 2.0f));	// (3,2)
		tc.emplace_back(ConvertTexCoord(4.0f, 1.0f));	// (4,1)
		tc.emplace_back(ConvertTexCoord(4.0f, 2.0f));	// (4,2)

		normals_axes.emplace_back(Vec3{ side,	 0,		 0 }, Vec3{ side*2.5f ,	 0,			 0 }, Colors::Blue); // X-axis
		normals_axes.emplace_back(Vec3{ -side,	 0,		 0 }, Vec3{ -side * 1.5f ,	 0,			 0 }, Colors::LightGray); // left
		normals_axes.emplace_back(Vec3{ 0,		 side,	 0 }, Vec3{ 0,			 side * 2.5f ,	 0 }, Colors::Green); // Y-axis
		normals_axes.emplace_back(Vec3{ 0,		-side,	 0 }, Vec3{ 0,			-side * 1.5f ,	 0 }, Colors::LightGray); // bottom
		normals_axes.emplace_back(Vec3{ 0,		 0,		 side }, Vec3{ 0,			 0,			 side * 2.5f }, Colors::Red); // Z-axis
		normals_axes.emplace_back(Vec3{ 0,		 0,		-side }, Vec3{ 0,			 0,			-side * 1.5f }, Colors::LightGray); // front

		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
			verts[i].tc = tc[i];
		}

		return{
			std::move(verts),
			{	// Ordered triangle indices
				 3, 7, 8,	 8, 4, 3,	// right (X-axis)
				10,12,13,	13,11,10,	// left
				 2, 6, 7, 	 7, 3, 2,	// top (Y-axis)
				 4, 8, 9,	 9, 5, 4,	// bottom
				 7,10,11,	11, 8, 7,	// back (Z-axis)
				 2, 3, 1,    1, 0, 2	// front
			},
			normals_axes,
			{	// Ordered side indiced
				3,7,	7,8,	8,4,	4,3,
				10,12,  12,13,  13,11,  11,10,
				0,4,    1,3,	7,10,	8,11
			}
		};
	}
};



