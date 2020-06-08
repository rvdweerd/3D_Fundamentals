#pragma once

#include "Pipeline.h"

class TextureEffect
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
		Vertex(const Vec3& pos, const Vertex& v)
			:
			pos(pos),
			tc(v.tc)
		{
		}

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
		Vertex& operator*=(const float rhs)
		{
			pos *= rhs;
			tc *= rhs;
			return *this;
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
	class PixelShader
	{
	public:
		//template<class Input>
		//Color operator()(const Input& in) const
		Color operator()(const Vertex& in) const
		{
			return pTex->GetPixel(
				(unsigned int)std::min(in.tc.x * tex_width +0.5f , tex_xclamp),
				(unsigned int)std::min(in.tc.y * tex_height+0.5f, tex_yclamp));
				//int(std::fmod(std::max(in.tc.x * tex_width, 0.0f), tex_xclamp)),
				//int(std::fmod(std::max(in.tc.y * tex_height, 0.0f), tex_yclamp)));
		}
		void BindTexture(const std::wstring& filename)
		{
			pTex = std::make_unique<Surface>(Surface::FromFile(filename));
			tex_height = (float)pTex->GetHeight();
			tex_width = (float)pTex->GetWidth();
			tex_xclamp = tex_width - 1.0f;
			tex_yclamp = tex_height - 1.0f;
		}
		void BindTexture(const Color c)
		{
			pTex = std::make_unique<Surface>(2, 2);
			pTex->Clear(c);
			/*pTex->PutPixel(0, 0, Colors::Red);
			pTex->PutPixel(1, 3, Colors::Red);
			pTex->PutPixel(4, 2, Colors::Red);
			pTex->PutPixel(0, 1, Colors::Blue);
			pTex->PutPixel(1, 2, Colors::Blue);
			pTex->PutPixel(1, 0, Colors::Green);
			pTex->PutPixel(4, 1, Colors::Green);
			pTex->PutPixel(1, 1, Colors::Yellow);*/
			tex_height = (float)pTex->GetHeight();
			tex_width =  (float)pTex->GetWidth();
			tex_yclamp = tex_height - 1.0f;
			tex_xclamp = tex_width - 1.0f;

		}
		std::unique_ptr<Surface> pTex;
		float tex_width;
		float tex_height;
		float tex_xclamp;
		float tex_yclamp;
	};
public:
	PixelShader ps;
};
