#pragma once
#include "Vec3.h"
#include <vector>
#include "IndexedTriangleList.h"
//#include "TexVertex.h"

class Rectangle
{
public:
	template<class V>
	static IndexedTriangleList<V> GetPlain(float size = 1.0f)
	{
		const float side = size / 2.0f;
		std::vector<Vec3> vertices;
		std::vector<Axis> normals_axes;
		vertices.emplace_back(-side, -side, 0.f);	// #0
		vertices.emplace_back(+side, -side, 0.f);	// #1
		vertices.emplace_back(-side, +side, 0.f);	// #2		
		vertices.emplace_back(+side, +side, 0.f);	// #3
		vertices.emplace_back(-side, -side, 0.f);	// #4
		vertices.emplace_back(+side, -side, 0.f);	// #5
		vertices.emplace_back(-side, +side, 0.f);	// #6		
		vertices.emplace_back(+side, +side, 0.f);	// #7
		
		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
		}

		return{
			std::move(verts),
			{	// Ordered triangle indices
				 0, 2, 3,    3, 1, 0	// front
				 7, 6, 4,    4, 5, 7	// back
			},
			normals_axes,
			{	// Ordered side indices
				0,1, 1,3, 3,2, 3,4
			}
		};

	}
	template<class V>
	static IndexedTriangleList<V> GetSkinned(float size = 1.0f)
	{
		const float side = size / 2.0f;
		const auto ConvertTexCoord = [](float u, float v)
		{
			return Vec2{ u / 2.0f,v }; // SkinSurface
		};

		std::vector<Vec3> vertices;
		std::vector<Vec2> tc;
		std::vector<Axis> normals_axes;

		const float texDim = 1.0f; // used for texture wrapping

		vertices.emplace_back(-side, -side, 0.f);	// #0
		vertices.emplace_back(+side, -side, 0.f);	// #1
		vertices.emplace_back(-side, +side, 0.f);	// #2		
		vertices.emplace_back(+side, +side, 0.f);	// #3
		vertices.emplace_back(-side, -side, 0.f);	// #4
		vertices.emplace_back(+side, -side, 0.f);	// #5
		vertices.emplace_back(-side, +side, 0.f);	// #6		
		vertices.emplace_back(+side, +side, 0.f);	// #7

		tc.emplace_back(ConvertTexCoord(0.0f, 1.0f));	// (0,1)
		tc.emplace_back(ConvertTexCoord(1.0f, 1.0f));	// (1,1)	
		tc.emplace_back(ConvertTexCoord(0.0f, 0.0f));	// (0,0)
		tc.emplace_back(ConvertTexCoord(1.0f, 0.0f));	// (1,0)
		tc.emplace_back(ConvertTexCoord(2.0f, 1.0f));	// (2,1)
		tc.emplace_back(ConvertTexCoord(1.0f, 1.0f));	// (1,1)
		tc.emplace_back(ConvertTexCoord(2.0f, 0.0f));	// (2,0)
		tc.emplace_back(ConvertTexCoord(1.0f, 0.0f));	// (1,0)

		normals_axes.emplace_back(Vec3{ 0	,	 0,		 0 },	Vec3{ side * 2.5f ,	 0,				 0			}, Colors::Blue);		// X-axis
		normals_axes.emplace_back(Vec3{ 0,		 0,		 0 },	Vec3{ 0,			 side * 2.5f ,	 0			}, Colors::Green);		// Y-axis
		normals_axes.emplace_back(Vec3{ 0,		 0,		 0 },	Vec3{ 0,			 0,				 side * 2.5f}, Colors::Red);		// Z-axis
		normals_axes.emplace_back(Vec3{ 0,		 0,		0 },Vec3{ 0,			 0,				-side * 1.5f}, Colors::LightGray);	// front
		normals_axes.emplace_back(Vec3{ 0,		 0,		 0 }, Vec3{ 0,			 0,				 side * 1.5f }, Colors::LightGray);	// back

		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].pos = vertices[i];
			verts[i].tc = tc[i];
		}

		return{
			std::move(verts),
			{	// Ordered triangle indices
				 0, 2, 3,    3, 1, 0,	// front
				 7, 6, 4,    4, 5, 7	// back
			},
			normals_axes,
			{	// Ordered side indiced
				0,1, 1,3, 3,2, 3,4
			}
		};
	}
};



