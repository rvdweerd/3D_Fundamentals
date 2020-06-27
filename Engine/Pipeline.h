#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include "Graphics.h"
#include "PubeScreenTransformer.h"
#include "Mat3.h"
#include <Vector>
#include "Triangle.h"
#include "IndexedTriangleList.h"
#include "ZBuffer.h"
//#include "GeoMath.h"


template<class Effect>
class Pipeline
{
public:
	typedef typename Effect::Vertex Vertex;
	Pipeline(Graphics& gfx)
		:
		gfx(gfx),
		zb(Graphics::ScreenWidth,Graphics::ScreenHeight)
	{}
	std::vector<Vertex> Draw(IndexedTriangleList<Vertex>& triList)
	{
		std::vector<Vertex> ret = ProcessVertices(triList.vertices, triList.indices, triList.sides);
		ProcessAxes(triList.normals_axes);
		return ret;
	}
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
	void BeginFrame()
	{
		zb.Clear();
	}
	IntersectData TrianglesIntersect(Vec3 v1_0, Vec3 v1_1, Vec3 v1_2, const Vec3& v2_0, const Vec3& v2_1, const Vec3& v2_2 )
	{
		// Plane equation for Plane2
		Vec3 N2 = (v2_1 - v2_0) % (v2_2 - v2_0);
		//N2 /= N2.Len();
		float d2 = -N2 * v2_0;

		// Plane equation for Plane1
		Vec3 N1 = (v1_1 - v1_0) % (v1_2 - v1_0);
		//N1 /= N1.Len();
		float d1 = -N1 * v1_0;

		// signed distances from T1 vertices to Plane2
		float d_v1_0 = N2 * v1_0 + d2;
		float d_v1_1 = N2 * v1_1 + d2;
		float d_v1_2 = N2 * v1_2 + d2;

		// Test: triangle 1 is coplanar
		//assert(!(d_v1_0 == 0 && d_v1_1 == 0 && d_v1_2 == 0));
		if ((d_v1_0 == 0 && d_v1_1 == 0 && d_v1_2 == 0)) return { {{0,0,2},{0,0,2} }, { 0,0,0 }, { 0,0,0 }};

		// Test: triangle 1 is not fully on 1 side of P2
		bool NoT1VertexOnPlane2 = (d_v1_0 != 0 && d_v1_1 != 0 && d_v1_2 != 0);
		bool T1DistancesHaveSameSign = (d_v1_0 > 0 && d_v1_1 > 0 && d_v1_2 > 0) || (d_v1_0 < 0 && d_v1_1 < 0 && d_v1_2 < 0);
		//assert(!(NoT1VertexOnPlane2 && T1DistancesHaveSameSign));
		if (NoT1VertexOnPlane2 && T1DistancesHaveSameSign) return { {{0,0,2},{0,0,2} }, { 0,0,0 }, { 0,0,0 } };

		// Ensure that v1_1 is solitary (two other vertices are on other side of T2)
		if (sgn(d_v1_0) == sgn(d_v1_1))
		{
			std::swap(d_v1_1, d_v1_2);
			//std::swap(v1_0, v1_2); NEED TO IMPLEMENT MOVE CONSTRUCTOR FOR Vec3 FOR THIS TO WORK
			Vec3 tmp = v1_1;
			v1_1 = v1_2;
			v1_2 = tmp;

			// Plane equation for Plane2
			N2 = (v2_1 - v2_0) % (v2_2 - v2_0);
			//N2 /= N2.Len();
			d2 = -N2 * v2_0;
			// Plane equation for Plane1
			N1 = (v1_1 - v1_0) % (v1_2 - v1_0);
			//N1 /= N1.Len();
			d1 = -N1 * v1_0;
			// signed distances from T1 vertices to Plane2
			//d_v1_0 = N2 * v1_0 + d2;
			//d_v1_1 = N2 * v1_1 + d2;
			//d_v1_2 = N2 * v1_2 + d2;
		}
		else if (sgn(d_v1_2) == sgn(d_v1_1))
		{
			std::swap(d_v1_1, d_v1_0);
			Vec3 tmp = v1_0;
			v1_0 = v1_1;
			v1_1 = tmp;

			// Plane equation for Plane2
			N2 = (v2_1 - v2_0) % (v2_2 - v2_0);
			//N2 /= N2.Len();
			d2 = -N2 * v2_0;
			// Plane equation for Plane1
			N1 = (v1_1 - v1_0) % (v1_2 - v1_0);
			//N1 /= N1.Len();
			d1 = -N1 * v1_0;
			// signed distances from T1 vertices to Plane2
			d_v1_0 = N2 * v1_0 + d2;
			d_v1_1 = N2 * v1_1 + d2;
			d_v1_2 = N2 * v1_2 + d2;
		}

		// Intersection line L = O + t*D (O = any point on line, D = direction), this represents the nullspace Ax=0)
		Vec3 D = N1 % N2;
		D /= D.Len();
		// Particular point on line (Ax=b, particular solution)
		// Find largest entry of D
		Vec3 O;
		if (abs(D.x) >= abs(D.y) && abs(D.x) >= abs(D.z)) // D.x largest component
		{
			float a = N1.y, b = N1.z;
			float c = N2.y, d = N2.z;
			float det = 1 / (a * d - b * c);
			float O_y = det * (d * (-d1) - b * (-d2));
			float O_z = det * (-c * (-d1) + a * (-d2));
			O = { 0,O_y,O_z };
		}
		else if (abs(D.y) >= abs(D.x) && abs(D.y) >= abs(D.z)) // D.y largest component
		{
			float a = N1.x, b = N1.z;
			float c = N2.x, d = N2.z;
			float det = 1 / (a * d - b * c);
			float O_x = det * (d * (-d1) - b * (-d2));
			float O_z = det * (-c * (-d1) + a * (-d2));
			O = { O_x,0,O_z };
		}
		else // D.z largest component
		{
			float a = N1.x, b = N1.y;
			float c = N2.x, d = N2.y;
			float det = 1 / (a * d - b * c);
			float O_x = det * (d * (-d1) - b * (-d2));
			float O_y = det * (-c * (-d1) + a * (-d2));
			O = { O_x,O_y,0 };
		}
		assert(N1 * (O + D * 20) + d1 < 0.0001f);
		assert(N2 * (O + D * 10) + d2 < 0.0001f);

		// Project vertices of T1 onto Intersection Line L
		Vec3 p_v1_0 = D * (D * (v1_0 - O)) / (D * D) + O;
		Vec3 p_v1_1 = D * (D * (v1_1 - O)) / (D * D) + O;
		Vec3 p_v1_2 = D * (D * (v1_2 - O)) / (D * D) + O;

		// Interpolation to find t1
		//float S = abs((p_v1_1 - p_v1_0).Len());
		float a = abs((p_v1_0 - v1_0).Len());
		float b = abs((p_v1_1 - v1_1).Len());
		float p = a / (b + a);
		//Vec3 t1 = p_v1_1 + (p_v1_0 - p_v1_1) * p;
		//Vec3 t1 = p_v1_0 + (p_v1_1 - p_v1_0) * p;
		//Vec3 t1 = p_v1_0 + (p_v1_1 - p_v1_0) * p;
		Vec3 t1 = p_v1_0 + (p_v1_1 - p_v1_0) * (d_v1_0 / (d_v1_0 - d_v1_1));
		
		// Interpolation to find t2
		//S = abs((p_v1_1 - p_v1_2).Len());
		a = abs((p_v1_2 - v1_2).Len());
		b = abs((p_v1_1 - v1_1).Len());
		p = a  / (b + a);
		
		//Vec3 t2 = p_v1_2 + (p_v1_1 - p_v1_2) * p;
		//Vec3 t2 = p_v1_2 + (p_v1_1 - p_v1_2) * p ;
		Vec3 t2 = p_v1_2 + (p_v1_1 - p_v1_2) * (d_v1_2 / (d_v1_2 - d_v1_1));
		return { {t1,t2},O,D };
	}
	void DrawLine(Vec3 p1, Vec3 p2, Color col)
	{
		pst.Transform(p1, true);
		pst.Transform(p2, true);
		gfx.DrawLine(p1, p2, col);
	}
private:
	std::vector<Vertex> ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices, const std::vector<size_t>& sides)
	{
		// create vertex vector for vs output
		std::vector<Vertex> verticesOut;

		// transform vertices using matrix + vector
		for (const auto& v : vertices)
		{
			verticesOut.emplace_back(v.pos * rotation + translation, v);
		}
		
		// assemble triangles from stream of indices and vertices
		AssembleTriangles(verticesOut, indices);
		//ProcessSidesWireFrame(verticesOut, sides);
		//std::vector<Vertex> ret;
		//for (const auto& v : verticesOut)
		//{
		//	ret.push_back(v);
		//}
		return verticesOut;
	}
	void ProcessSidesWireFrame(const std::vector<Vertex>& vertices, const std::vector<size_t>& sides)
	{
		if (sides.size() > 0)
		{
			size_t nLinePoints = sides.size();
			for (size_t i = 0, end = nLinePoints - 2; i <= end; i += 2)
			{
				gfx.DrawLine(pst.GetTransformed(vertices[sides[i]].pos, true), pst.GetTransformed(vertices[sides[i + 1]].pos, true), Colors::White);
			}
		}
	}
	void ProcessAxes(const std::vector<Axis>& normals_axes)
	{
		if (normals_axes.size() > 0)
		{
			std::vector<Axis> axesOut;

			for (const auto& a : normals_axes)
			{
				axesOut.emplace_back(a.p0 * rotation + translation, a.p1 * rotation + translation, a.col);
			}
			for (auto& a : axesOut)
			{
				Vec2 p0 = pst.GetTransformed(a.p0, true);
				Vec2 p1 = pst.GetTransformed(a.p1, true);

				gfx.DrawLine(p0, p1, a.col);
			}
		}
	}
	void AssembleTriangles(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		// assemble triangles in the stream and process
		for (size_t i = 0, end = indices.size() / 3;
			i < end; i++)
		{
			// determine triangle vertices via indexing
			const auto& v0 = vertices[indices[i * 3]];
			const auto& v1 = vertices[indices[i * 3 + 1]];
			const auto& v2 = vertices[indices[i * 3 + 2]];
			// cull backfacing triangles with cross product (%) shenanigans
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * v0.pos <= 0.0f)
			{
				// process 3 vertices into a triangle
				ProcessTriangle(v0, v1, v2);
			}
		}
	}
	void ProcessTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		PostProcessTriangleVertices(Triangle<Vertex>{ v0, v1, v2 });
	}
	void PostProcessTriangleVertices(Triangle<Vertex>& triangle)
	{
		// perspective divide and screen transform for all 3 vertices
		pst.Transform(triangle.v0,true);
		pst.Transform(triangle.v1,true);
		pst.Transform(triangle.v2,true);

		// draw the triangle
		DrawTriangle(triangle);
		
	}
	//void DrawTriangleEdges(const Triangle<Vertex>& triangle)
public:
	void DrawTriangleEdges(const IndexedTriangleList<Vertex>& triList)
	{
		auto vertices = triList.vertices;
		auto indices = triList.indices;
		std::vector<Vertex> verticesOut;
		for (const auto& v : vertices)
		{
			verticesOut.emplace_back(v.pos * rotation + translation, v);
		}
		// assemble triangles in the stream and process
		for (size_t i = 0, end = indices.size() / 3;
			i < end; i++)
		{
			// determine triangle vertices via indexing
			auto v0 = verticesOut[indices[i * 3]];
			auto v1 = verticesOut[indices[i * 3 + 1]];
			auto v2 = verticesOut[indices[i * 3 + 2]];
			// cull backfacing triangles with cross product (%) shenanigans
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * v0.pos <= 0.0f)
			{
				// process 3 vertices into a triangle
				pst.Transform(v0, true);
				pst.Transform(v1, true);
				pst.Transform(v2, true);
				DrawTriangleEdgesOne(Triangle<Vertex>{v0, v1, v2});
			}
		}
	}
private:
	void DrawTriangleExecute(const Vertex& v0, const Vertex& v1)
	{
		auto it0 = v0;
		auto it1 = v1;
		float dx = it1.pos.x - it0.pos.x;
		float dy = it1.pos.y - it0.pos.y;
		{
			if (abs(dx) <= abs(dy)) // steep line (iterate over x)
			{
				if (dy < 0)
				{
					std::swap(it0, it1);
					dx = -dx;
					dy = -dy;
				}
				const auto dLine = (it1 - it0) / dy;
				auto iLine = it0;

				// prestep scanline interpolant
				const int xStart = (int)ceil(it0.pos.x - 0.5f);
				iLine += dLine * (float(xStart) + 0.5f - it0.pos.x);

				for (; iLine.pos.y < it1.pos.y; iLine += dLine)
				{
					{
						const float z = 1.0f / iLine.pos.z;
						if (zb.Test((int)iLine.pos.x, (int)iLine.pos.y, z).second)
						{
							//const auto attr = iLine * z;
							// perform texture lookup, clamp, and write pixel
							gfx.PutPixel((int)iLine.pos.x, (int)iLine.pos.y, Colors::Blue);
						}
					}
				}
			}
			else // shallow line (iterate over y)
			{
				if (dx < 0)
				{
					std::swap(it0, it1);
					dx = -dx;
					dy = -dy;
				}
				const auto dLine = (it1 - it0) / dx;
				auto iLine = it0;

				// prestep scanline interpolant
				const int xStart = (int)ceil(it0.pos.x - 0.5f);
				iLine += dLine * (float(xStart) + 0.5f - it0.pos.x);

				for (; iLine.pos.x < it1.pos.x; iLine += dLine)
				{
					{
						const float z = 1.0f / iLine.pos.z;
						if (zb.Test((int)iLine.pos.x, (int)iLine.pos.y, z).second)
						{
							//const auto attr = iLine * z;
							// perform texture lookup, clamp, and write pixel
							gfx.PutPixel((int)iLine.pos.x, (int)iLine.pos.y, Colors::Blue);
						}
					}
				}
			}
		}
	}
	void DrawTriangleEdgesOne(const Triangle<Vertex>& triangle)
	{
		//gfx.DrawLine(triangle.v0.pos, triangle.v1.pos, Colors::Blue);
		//gfx.DrawLine(triangle.v1.pos, triangle.v2.pos, Colors::Blue);
		//gfx.DrawLine(triangle.v0.pos, triangle.v2.pos, Colors::Blue);
		//return;
		DrawTriangleExecute(triangle.v0, triangle.v1);
		DrawTriangleExecute(triangle.v1, triangle.v2);
		//DrawTriangleExecute(triangle.v0, triangle.v2);
		return;
	}
	void DrawTriangle(const Triangle<Vertex>& triangle)
	{
		// using pointers so we can swap (for sorting purposes)
		const Vertex* pv0 = &triangle.v0;
		const Vertex* pv1 = &triangle.v1;
		const Vertex* pv2 = &triangle.v2;

		// sorting vertices by y
		if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);
		if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
		if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);

		if (pv0->pos.y == pv1->pos.y) // natural flat top
		{
			// sorting top vertices by x
			if (pv1->pos.x < pv0->pos.x) std::swap(pv0, pv1);

			DrawFlatTopTriangle(*pv0, *pv1, *pv2);
		}
		else if (pv1->pos.y == pv2->pos.y) // natural flat bottom
		{
			// sorting bottom vertices by x
			if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2);

			DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
		}
		else // general triangle
		{
			// find splitting vertex interpolant
			const float alphaSplit =
				(pv1->pos.y - pv0->pos.y) /
				(pv2->pos.y - pv0->pos.y);
			const auto vi = interpolate(*pv0, *pv2, alphaSplit);

			if (pv1->pos.x < vi.pos.x) // major right
			{
				DrawFlatBottomTriangle(*pv0, *pv1, vi);
				DrawFlatTopTriangle(*pv1, vi, *pv2);
			}
			else // major left
			{
				DrawFlatBottomTriangle(*pv0, vi, *pv1);
				DrawFlatTopTriangle(vi, *pv1, *pv2);
			}
		}
	}
	void DrawFlatTopTriangle(const Vertex& it0,
		const Vertex& it1,
		const Vertex& it2)
	{
		// calulcate dVertex / dy
// change in interpolant for every 1 change in y
		const float delta_y = it2.pos.y - it0.pos.y;
		const auto dit0 = (it2 - it0) / delta_y;
		const auto dit1 = (it2 - it1) / delta_y;

		// create right edge interpolant
		auto itEdge1 = it1;

		// call the flat triangle render routine
		DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
	}
	void DrawFlatBottomTriangle(const Vertex& it0,
		const Vertex& it1,
		const Vertex& it2)
	{
		// calulcate dVertex / dy
		// change in interpolant for every 1 change in y
		const float delta_y = it2.pos.y - it0.pos.y;
		const auto dit0 = (it1 - it0) / delta_y;
		const auto dit1 = (it2 - it0) / delta_y;

		// create right edge interpolant
		auto itEdge1 = it0;

		// call the flat triangle render routine
		DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
	}
	void DrawFlatTriangle(const Vertex& it0,
		const Vertex& it1,
		const Vertex& it2,
		const Vertex& dv0,
		const Vertex& dv1,
		Vertex itEdge1)
	{
		// create edge interpolant for left edge (always v0)
		auto itEdge0 = it0;

		// calculate start and end scanlines
		const int yStart = (int)ceil(it0.pos.y - 0.5f);
		const int yEnd = (int)ceil(it2.pos.y - 0.5f); // the scanline AFTER the last line drawn

		// do interpolant prestep
		itEdge0 += dv0 * (float(yStart) + 0.5f - it0.pos.y);
		itEdge1 += dv1 * (float(yStart) + 0.5f - it0.pos.y);

		for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
		{
			// calculate start and end pixels
			const int xStart = (int)ceil(itEdge0.pos.x - 0.5f);
			const int xEnd = (int)ceil(itEdge1.pos.x - 0.5f); // the pixel AFTER the last pixel drawn

			// create scanline interpolant startpoint
			// (some waste for interpolating x,y,z, but makes life easier not having
			//  to split them off, and z will be needed in the future anyways...)
			auto iLine = itEdge0;
						
			// calculate delta scanline interpolant / dx
			const float dx = itEdge1.pos.x - itEdge0.pos.x;
			const auto diLine = (itEdge1 - iLine) / dx;

			// prestep scanline interpolant
			iLine += diLine * (float(xStart) + 0.5f - itEdge0.pos.x);

			for (int x = xStart; x < xEnd; x++, iLine += diLine)
			{
				// recover interpolated z from interpolated 1/z
				const float z = 1.0f/ iLine.pos.z;

				auto p = zb.TestAndSet(x, y, z);
				if (p.first) // visible
				{
					const auto attr = iLine * z;
					// perform texture lookup, clamp, and write pixel
					if (p.second) // on cutting edge
					{
						//gfx.PutPixel(x, y, Colors::Yellow);
					}
					else
					{
						//gfx.PutPixel(x, y, effect.ps(attr));
						//if (x <= xStart+0.5 || x>=xEnd-1.5 || y <= yStart+0.5 || y>=yEnd-1.5) gfx.PutPixel(x, y, Colors::Blue);
						///*else*/ gfx.PutPixel(x, y, Colors::Black);
					}
				}
			}			
		}
	}
private:
	Graphics& gfx;
	PubeScreenTransformer pst;
	Mat3 rotation;
	Vec3 translation;
	ZBuffer zb;
public:
	Effect effect;
};