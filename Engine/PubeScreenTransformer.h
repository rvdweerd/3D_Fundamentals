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
	Vec3& Transform( Vec3& v , bool divide) const
	{
		float z_div = 1.0f;
		if (divide) z_div = 1.0f / v.z;
		
		v.x = (v.x * z_div + 1.0f) * xFactor;
		v.y = (-v.y * z_div + 1.0f) * yFactor;
		return v;
	}
	Vec3 GetTransformed( const Vec3& v ) const
	{
		return Transform( Vec3( v ) ,true);
	}
private:
	float xFactor;
	float yFactor;
};