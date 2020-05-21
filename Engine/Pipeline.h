#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include "Graphics.h"
#include "PubeScreenTransformer.h"
#include "Mat3.h"
#include <Vector>
#include "Triangle.h"
#include "IndexedTriangleList.h"

class Pipeline
{
public:
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos, const Vec2& tc)
			:
			pos(pos),
			tc(tc)
		{}
		Vertex InterpolateTo(const Vertex& dest, float alpha) const
		{
			return {
				pos.InterpolateTo(dest.pos,alpha),
				tc.InterpolateTo(dest.tc, alpha)
			};
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return {
				pos - rhs.pos,
				tc - rhs.tc
			};
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return {
				pos + rhs.pos,
				tc + rhs.tc
			};
		}
		Vertex operator/(const float k) const
		{
			return {
				pos / k,
				tc / k
			};
		}
		Vertex operator*(const float k) const
		{
			return {
				pos * k,
				tc * k
			};
		}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			tc += rhs.tc;
			return *this;
		}
		Vec3 pos;
		Vec2 tc;
	};
	Pipeline(Graphics& gfx)
		:
		gfx(gfx)
	{}
	void Draw(IndexedTriangleList<Vertex>& triList)
	{
		ProcessVertices(triList.vertices, triList.indices);
		ProcessAxes(triList.normals_axes);
	}
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
	void BindTexture(const std::wstring& filename)
	{
		pTex = std::make_unique<Surface>(Surface::FromFile(filename));
	}
private:
	void ProcessAxes(const std::vector<Axis>& normals_axes)
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
	void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		// create vertex vector for vs output
		std::vector<Vertex> verticesOut;

		// transform vertices using matrix + vector
		for (const auto& v : vertices)
		{
			verticesOut.emplace_back(v.pos * rotation + translation, v.tc);
		}
		
		// assemble triangles from stream of indices and vertices
		AssembleTriangles(verticesOut, indices);
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
		pst.Transform(triangle.v0.pos,true);
		pst.Transform(triangle.v1.pos,true);
		pst.Transform(triangle.v2.pos,true);

		// draw the triangle
		DrawTriangle(triangle);
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

		// prepare clamping constants
		const float tex_width = float(pTex->GetWidth());
		const float tex_height = float(pTex->GetHeight());
		const float tex_xclamp = tex_width - 1.0f;
		const float tex_yclamp = tex_height - 1.0f;

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
				// perform texture lookup, clamp, and write pixel
				gfx.PutPixel(x, y, pTex->GetPixel(
					//(unsigned int)std::min(iLine.t.x * tex_width + 0.5f, tex_xclamp),
					//(unsigned int)std::min(iLine.t.y * tex_height + 0.5f, tex_yclamp)

					int(std::fmod(std::max(iLine.tc.x * tex_width, 0.0f), tex_xclamp)),
					int(std::fmod(std::max(iLine.tc.y * tex_height, 0.0f), tex_yclamp))

				));
			}
		}
	}
private:
	Graphics& gfx;
	PubeScreenTransformer pst;
	Mat3 rotation;
	Vec3 translation;
	std::unique_ptr<Surface> pTex;
};