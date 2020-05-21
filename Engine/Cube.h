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
		
		vertices.emplace_back(side, side, side);
		vertices.emplace_back(side, side, -side);
		vertices.emplace_back(side, -side, side);
		vertices.emplace_back(side, -side, -side);
		vertices.emplace_back(-side, side, side);
		vertices.emplace_back(-side, side, -side);
		vertices.emplace_back(-side, -side, side);
		vertices.emplace_back(-side, -side, -side);
		tc.emplace_back(ConvertTexCoord(0.0f,   0.0f));		//0
		tc.emplace_back(ConvertTexCoord(texDim, 0.0f));		//1
		tc.emplace_back(ConvertTexCoord(0.0f,   texDim));	//2
		tc.emplace_back(ConvertTexCoord(texDim, texDim));	//3
		tc.emplace_back(ConvertTexCoord(texDim, 0.0f));		//4
		tc.emplace_back(ConvertTexCoord(0.0f,   0.0f));		//5
		tc.emplace_back(ConvertTexCoord(texDim, texDim));	//6
		tc.emplace_back(ConvertTexCoord(0.0f,   texDim));	//7

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
			std::move(verts),{
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
};



