#pragma once
#include "Vec3.h"
#include <vector>
#include "IndexedTriangleList.h"
//#include "TexVertex.h"

class Cube
{
public:
	template<class V>
	static IndexedTriangleList<V> GetPlain(float size = 1.0f)
	{
		const float side = size / 2.0f;
		std::vector<Vec3> vertices;
		std::vector<Axis> normals_axes;
		vertices.emplace_back(-side, -side, -side);	// #0
		vertices.emplace_back(+side, -side, -side);	// #1
		vertices.emplace_back(-side, +side, -side); // #2		
		vertices.emplace_back(+side, +side, -side);	// #3
		vertices.emplace_back(-side, -side, +side);	// #4
		vertices.emplace_back(+side, -side, +side);	// #5
		vertices.emplace_back(-side, +side, +side);	// #6
		vertices.emplace_back(+side, +side, +side);	// #7
		
		//normals_axes.emplace_back(Vec3{ side,	 0,		 0 }, Vec3{ side * 2.5f ,	 0,			 0 }, Colors::Blue); // X-axis
		//normals_axes.emplace_back(Vec3{ -side,	 0,		 0 }, Vec3{ -side * 1.5f ,	 0,			 0 }, Colors::LightGray); // left
		//normals_axes.emplace_back(Vec3{ 0,		 side,	 0 }, Vec3{ 0,			 side * 2.5f ,	 0 }, Colors::Green); // Y-axis
		//normals_axes.emplace_back(Vec3{ 0,		-side,	 0 }, Vec3{ 0,			-side * 1.5f ,	 0 }, Colors::LightGray); // bottom
		//normals_axes.emplace_back(Vec3{ 0,		 0,		 side }, Vec3{ 0,			 0,			 side * 2.5f }, Colors::Red); // Z-axis
		//normals_axes.emplace_back(Vec3{ 0,		 0,		-side }, Vec3{ 0,			 0,			-side * 1.5f }, Colors::LightGray); // front
		
		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
		}

		return{
			std::move(verts),
			{	// Ordered triangle indices
				 3, 7, 5,	 5, 1, 3,	// right (X-axis)
				 6, 2, 0,	 0, 4, 6,	// left
				 2, 6, 7, 	 7, 3, 2,	// top (Y-axis)
				 1, 5, 4,	 4, 0, 1,	// bottom
				 7, 6, 4,	 4, 5, 7,	// back (Z-axis)
				 2, 3, 1,    1, 0, 2	// front
			},
			normals_axes,
			{	// Ordered side indices
				3,7,	7,5,	5,1,	1,3,
				6,2,    2,0,    0,4,    4,6,
				2,3,    6,7,	4,5,	0,1
			}
		};

	}
	template<class V>
	static IndexedTriangleList<V> GetIndependentFaces(float size = 1.0f)
	{
		const float side = size / 2.0f;
		std::vector<Vec3> vertices;
		std::vector<Axis> normals_axes;
		vertices.emplace_back(-side, -side, -side);	// #0 FRONT FACE	0
		vertices.emplace_back(-side, +side, -side); // #2				1
		vertices.emplace_back(+side, +side, -side);	// #3				2
		vertices.emplace_back(+side, -side, -side);	// #1				3

		vertices.emplace_back(+side, -side, -side);	// #1 RIGHT FACE	4
		vertices.emplace_back(+side, +side, -side);	// #3				5
		vertices.emplace_back(+side, +side, +side);	// #7				6
		vertices.emplace_back(+side, -side, +side);	// #5				7

		vertices.emplace_back(-side, -side, +side);	// #4 BACK FACE		8
		vertices.emplace_back(+side, -side, +side);	// #5				9
		vertices.emplace_back(+side, +side, +side);	// #7				10
		vertices.emplace_back(-side, +side, +side);	// #6				11

		vertices.emplace_back(-side, -side, -side);	// #0 LEFT FACE		12
		vertices.emplace_back(-side, -side, +side);	// #4				13
		vertices.emplace_back(-side, +side, +side);	// #6				14
		vertices.emplace_back(-side, +side, -side); // #2				15

		vertices.emplace_back(-side, +side, -side); // #2 TOP FACE		16
		vertices.emplace_back(-side, +side, +side);	// #6				17
		vertices.emplace_back(+side, +side, +side);	// #7				18
		vertices.emplace_back(+side, +side, -side);	// #3				19

		vertices.emplace_back(-side, -side, -side);	// #0 BOTTOM		20
		vertices.emplace_back(+side, -side, -side);	// #1				21
		vertices.emplace_back(+side, -side, +side);	// #5				22
		vertices.emplace_back(-side, -side, +side);	// #4				23

		//normals_axes.emplace_back(Vec3{ side,	 0,		 0 }, Vec3{ side * 2.5f ,	 0,			 0 }, Colors::Blue); // X-axis
		//normals_axes.emplace_back(Vec3{ -side,	 0,		 0 }, Vec3{ -side * 1.5f ,	 0,			 0 }, Colors::LightGray); // left
		//normals_axes.emplace_back(Vec3{ 0,		 side,	 0 }, Vec3{ 0,			 side * 2.5f ,	 0 }, Colors::Green); // Y-axis
		//normals_axes.emplace_back(Vec3{ 0,		-side,	 0 }, Vec3{ 0,			-side * 1.5f ,	 0 }, Colors::LightGray); // bottom
		//normals_axes.emplace_back(Vec3{ 0,		 0,		 side }, Vec3{ 0,			 0,			 side * 2.5f }, Colors::Red); // Z-axis
		//normals_axes.emplace_back(Vec3{ 0,		 0,		-side }, Vec3{ 0,			 0,			-side * 1.5f }, Colors::LightGray); // front

		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
		}

		return{
			std::move(verts),
			{	// Ordered triangle indices
				  4, 5, 6,	 6, 7, 4,	// right (X-axis)
				 12,13,14,	14,15,12,	// left
				 16,17,18,	18,19,16,	// top (Y-axis)
				 20,21,22,	22,23,20,	// bottom
				  8, 9,10,	10,11, 8,	// back (Z-axis)
				  0, 1, 2,	 2, 3, 0	// front
			},
			normals_axes,
			{	// Ordered side indices
				3,7,	7,5,	5,1,	1,3,
				6,2,    2,0,    0,4,    4,6,
				2,3,    6,7,	4,5,	0,1
			}
		};

	}
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

		const float texDim =  1.0f; // used for texture wrapping
		
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



