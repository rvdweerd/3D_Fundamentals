#pragma once
#include "Colors.h"
#include "Graphics.h"

namespace SpriteEffect
{
	class Copy
	{
	public:
		void operator()(int xDest, int yDest, Color cSrc, Graphics& gfx) const
		{
			gfx.PutPixel(xDest, yDest, cSrc);
		}
	};
	class Chroma
	{
	public:
		Chroma(Color c)
			:
			chroma(c)
		{
		}
		void operator()(int xDest, int yDest, Color cSrc, Graphics& gfx) const
		{
			if (cSrc != chroma)
			{
				gfx.PutPixel(xDest, yDest, cSrc);
			}
		}
	private:
		Color chroma;
	};
	class Substitution
	{
	public:
		Substitution(Color c, Color s)
			:
			chroma(c),
			sub(s)
		{
		}
		void operator()(int xDest, int yDest, Color cSrc, Graphics& gfx) const
		{
			if (cSrc != chroma)
			{
				gfx.PutPixel(xDest, yDest, sub);
			}
		}
	private:
		Color chroma = Colors::Magenta;
		Color sub;
	};
	class Ghost
	{
	public:
		Ghost(Color c)
			:
			chroma(c)
		{
		}
		void operator()(const int xDest, const int yDest, Color cSrc, Graphics& gfx) const
		{
			const Color origPixelColor = cSrc;
			if (origPixelColor != chroma)
			{
				const Color backgroundColor = gfx.GetPixel(xDest, yDest);
				if (backgroundColor != Colors::Black)
				{
					const Color blendedPixelColor = Color(
						(origPixelColor.GetR() + backgroundColor.GetR()) / 2,
						(origPixelColor.GetG() + backgroundColor.GetG()) / 2,
						(origPixelColor.GetB() + backgroundColor.GetB()) / 2
					);
					gfx.PutPixel(xDest, yDest, blendedPixelColor);
				}
				else
				{
					gfx.PutPixel(xDest, yDest, origPixelColor);
				}
			}
		}
	private:
		Color chroma = Colors::Magenta;
	};
	class SubstitutionGhost
	{
	public:
		SubstitutionGhost(Color c, Color s)
			:
			chroma(c),
			sub(s)
		{
		}
		void operator()(int xDest, int yDest, Color cSrc, Graphics& gfx) const
		{
			if (cSrc != chroma)
			{
				const Color backgroundColor = gfx.GetPixel(xDest, yDest);
				if (backgroundColor != Colors::Black)
				{
					const Color blendedPixelColor = Color(
						(sub.GetR() + backgroundColor.GetR()) / 2,
						(sub.GetG() + backgroundColor.GetG()) / 2,
						(sub.GetB() + backgroundColor.GetB()) / 2
					);
					gfx.PutPixel(xDest, yDest, blendedPixelColor);
				}
				else
				{
					gfx.PutPixel(xDest, yDest, sub);
				}
			}
		}
	private:
		Color chroma = Colors::Magenta;
		Color sub;
	};
}