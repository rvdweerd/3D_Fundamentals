#pragma once
#include "Vec3.h"
#include <utility>

namespace GeoMath
{
	std::pair<Vec3, Vec3> TrianglesIntersect_(Vec3& v1_0, Vec3& v1_1, Vec3& v1_2, Vec3& v2_0, Vec3& v2_1, Vec3& v2_2)
	{
		// Plane equation for Plane2
		Vec3 N2_ = (v2_1 - v2_0) % (v2_2 - v2_0);
		//N2_ /= N2_.Len();
		float d2_ = -N2_ * v2_0;

		// Plane equation for Plane1
		Vec3 N1_ = (v1_1 - v1_0) % (v1_2 - v1_0);
		//N1_ /= N1_.Len();
		float d1_ = -N1_ * v1_0;

		// signed distances from T1 vertices to Plane2
		float d_v1_0 = N2_ * v1_0 + d2_;
		float d_v1_1 = N2_ * v1_1 + d2_;
		float d_v1_2 = N2_ * v1_2 + d2_;

		// Test: triangle 1 is coplanar
		//assert(!(d_v1_0 == 0 && d_v1_1 == 0 && d_v1_2 == 0));

		// Test: triangle 1 is not fully on 1 side of P2
		bool NoT1VertexOnPlane2 = (d_v1_0 != 0 && d_v1_1 != 0 && d_v1_2 != 0);
		bool T1DistancesHaveSameSign = (d_v1_0 > 0 && d_v1_1 > 0 && d_v1_2 > 0) || (d_v1_0 < 0 && d_v1_1 < 0 && d_v1_2 < 0);
		//assert(!(NoT1VertexOnPlane2 && T1DistancesHaveSameSign));

		// Ensure that v1_0 is solitary (two other vertices are on other side of T2)
		if (sgn(d_v1_0) == sgn(d_v1_1))
		{
			//std::swap(d_v1_0, d_v1_2);
			float tmpf = d_v1_0;
			d_v1_0 = d_v1_2;
			d_v1_2 = tmpf;

			//std::swap(v1_0, v1_2); NEED TO IMPLEMENT MOVE CONSTRUCTOR FOR Vec3 FOR THIS TO WORK
			Vec3 tmp = v1_0;
			v1_0 = v1_2;
			v1_2 = tmp;

			// Plane equation for Plane2
			N2_ = (v2_1 - v2_0) % (v2_2 - v2_0);
			//N2_ /= N2_.Len();
			d2_ = -N2_ * v2_0;
			// Plane equation for Plane1
			N1_ = (v1_1 - v1_0) % (v1_2 - v1_0);
			//N1_ /= N1_.Len();
			d1_ = -N1_ * v1_0;
			// signed distances from T1 vertices to Plane2
			//d_v1_0 = N2_ * v1_0 + d2_;
			//d_v1_1 = N2_ * v1_1 + d2_;
			//d_v1_2 = N2_ * v1_2 + d2_;
		}
		else if (sgn(d_v1_0) == sgn(d_v1_2))
		{
			//std::swap(d_v1_0, d_v1_1);
			float tmpf = d_v1_0;
			d_v1_0 = d_v1_1;
			d_v1_1 = tmpf;

			Vec3 tmp = v1_0;
			v1_0 = v1_1;
			v1_1 = tmp;

			// Plane equation for Plane2
			N2_ = (v2_1 - v2_0) % (v2_2 - v2_0);
			//N2_ /= N2_.Len();
			d2_ = -N2_ * v2_0;
			// Plane equation for Plane1
			N1_ = (v1_1 - v1_0) % (v1_2 - v1_0);
			//N1_ /= N1_.Len();
			d1_ = -N1_ * v1_0;
			// signed distances from T1 vertices to Plane2
			//d_v1_0 = N2_ * v1_0 + d2_;
			//d_v1_1 = N2_ * v1_1 + d2_;
			//d_v1_2 = N2_ * v1_2 + d2_;
		}

		// Intersection line L = O + t*D (O = any point on line, D = direction), this represents the nullspace Ax=0)
		Vec3 D_ = N1_ % N2_;
		D_ /= D_.Len();
		// Particular point on line (Ax=b, particular solution)
		// Find largest entry of D
		Vec3 O_;
		//if (std::abs((float)D.x) >= std::abs((float)D.y) && std::abs((float)D.x) >= std::abs((float)D.z)) // D.x largest component
		//{
		//	float a = N1_.y, b = N1_.z;
		//	float c = N2_.y, d = N2_.z;
		//	float det = 1 / (a * d - b * c);
		//	float O_y = det * (d * (-d1_) - b * (-d2_));
		//	float O_z = det * (-c * (-d1_) + a * (-d2_));
		//	O = { 0,O_y,O_z };
		//}
		//else if (std::abs((float)D.y) >= std::abs((float)D.x) && std::abs((float)D.y) >= std::abs((float)D.z)) // D.y largest component
		//{
		//	float a = N1_.x, b = N1_.z;
		//	float c = N2_.x, d = N2_.z;
		//	float det = 1 / (a * d - b * c);
		//	float O_x = det * (d * (-d1_) - b * (-d2_));
		//	float O_z = det * (-c * (-d1_) + a * (-d2_));
		//	O = { O_x,0,O_z };
		//}
		//else // D.z largest component
		//{
		//	float a = N1_.x, b = N1_.y;
		//	float c = N2_.x, d = N2_.y;
		//	float det = 1 / (a * d - b * c);
		//	float O_x = det * (d * (-d1_) - b * (-d2_));
		//	float O_y = det * (-c * (-d1_) + a * (-d2_));
		//	O = { O_x,O_y,0 };
		//}
		//assert(N1_ * (O_ + D_ * 20) == -d1_);
		//assert(N2_ * (O_ + D_ * 10) == -d2_);

		// Project vertices of T1 onto Intersection Line L
		Vec3 p_v1_0 = D_ * (D_ * (v1_0 - O_)) / (D_ * D_) + O_;
		Vec3 p_v1_1 = D_ * (D_ * (v1_1 - O_)) / (D_ * D_) + O_;
		Vec3 p_v1_2 = D_ * (D_ * (v1_2 - O_)) / (D_ * D_) + O_;

		// Interpolation to find t1
		float S_ = (p_v1_1 - p_v1_0).Len();
		float a_ = (p_v1_0 - v1_0).Len();
		float b_ = (p_v1_1 - v1_1).Len();
		float p_ = a_ * S_ / (b_ + a_);
		Vec3 t1_ = p_v1_1 + (p_v1_0 - p_v1_1) * p_;

		// Interpolation to find t2
		S_ = (p_v1_2 - p_v1_0).Len();
		a_ = (p_v1_0 - v1_0).Len();
		b_ = (p_v1_2 - v1_2).Len();
		p_ = a_ * S_ / (b_ + a_);
		Vec3 t2_ = p_v1_2 + (p_v1_0 - p_v1_2) * p_;
		return { t1_,t2_ };
	}
}