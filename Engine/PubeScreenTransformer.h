#pragma once
#include "Vec3.h"
#include "Graphics.h"

class PubeScreenTransformer
{
public:
	PubeScreenTransformer()
		:
		xFactor( float( Graphics::ScreenWidth ) / 2.0f ),
		yFactor( float( Graphics::ScreenHeight ) / 2.0f )
	{}
	template <class Vertex>
	Vertex& Transform( Vertex& v , bool divide) const
	{
		//float zInv = 1.0f;
		//if (divide) 
		float zInv = 1.0f / v.pos.z;
		v *= zInv;
		//v.pos.x *= zInv;
		//v.pos.y *= zInv;
		
		v.pos.x = (v.pos.x + 1.0f) * xFactor;
		v.pos.y = (-v.pos.y + 1.0f) * yFactor;
		v.pos.z = zInv;
		return v;
	}
	template <class Vertex>
	Vertex GetTransformed( const Vertex& v , bool divide) const
	{
		return Transform( ( v ) ,divide);
	}
	Vec3& Transform(Vec3& v, bool divide) const
	{
		//float z_div = 1.0f;
		//if (divide) 
		float z_div = 1.0f / (float)v.z;
		

		v.x = (v.x * z_div + 1.0f) * xFactor;
		v.y = (-v.y * z_div + 1.0f) * yFactor;
		return v;

		/*v *= z_div;
		v.x = (v.x  + 1.0f) * xFactor;
		v.y = (-v.y  + 1.0f) * yFactor;
		v.z = z_div;
		return v;*/

	}
	Vec3 GetTransformed(const Vec3& v, bool divide) const
	{
		return Transform(Vec3(v), divide);
	}
private:
	float xFactor;
	float yFactor;
};