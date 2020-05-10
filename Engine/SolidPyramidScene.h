#pragma once
#include "Scene.h"
#include "Mat3.h"
#include "PubeScreenTransformer.h"
#include "Pyramid.h"

class SolidPyramidScene : public Scene
{
public:
	SolidPyramidScene() = default;
	virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		Dtheta_x = 0.0f;
		Dtheta_y = 0.0f;
		Dtheta_z = 0.0f;
		if (kbd.KeyIsPressed('Q'))
		{
			Dtheta_x = AngVel * dt;
		}
		if (kbd.KeyIsPressed('W'))
		{
			Dtheta_y = AngVel * dt;
		}
		if (kbd.KeyIsPressed('E'))
		{
			Dtheta_z = AngVel * dt;
		}
		if (kbd.KeyIsPressed('A'))
		{
			Dtheta_x = -AngVel * dt;
		}
		if (kbd.KeyIsPressed('S'))
		{
			Dtheta_y = -AngVel * dt;
		}
		if (kbd.KeyIsPressed('D'))
		{
			Dtheta_z = -AngVel * dt;
		}
		if (kbd.KeyIsPressed('R'))
		{
			offset_z += 0.5f * dt;
		}
		if (kbd.KeyIsPressed('F'))
		{
			offset_z -= 0.5f * dt;
		}
		if (kbd.KeyIsPressed(VK_SPACE))
		{
			R_main = Mat3::Identity();
		}
		// Update rotation matrix based on keyboard input
		Mat3 rot = Mat3::RotationX(Dtheta_x) * Mat3::RotationZ(Dtheta_z) * Mat3::RotationY(Dtheta_y);
		R_main = rot * R_main;
	}
	virtual void Draw(Graphics& gfx) const override
	{
		auto triangles = pyramid.GetTriangles();

		//std::vector<Vec3> pubSpaceVerts;
		for (auto& v : triangles.vertices)
		{
			v *= R_main;
			v += { 0.0f, 0.0f, offset_z};// offset_z
		}
		for (auto& a : triangles.normals_axes)
		{
			a.p0 *= R_main;
			a.p1 *= R_main;
			a.p0 += { 0.0f, 0.0f, offset_z};// offset_z
			a.p1 += { 0.0f, 0.0f, offset_z};// offset_z
		}
		// Apply backface culling
		for (size_t i = 0; i < triangles.indices.size() / 3; i++)
		{
			// Get transformed surface normal (brought to focal point Origin)
			Vec3 sn = triangles.normals_axes[i ].p1 - triangles.normals_axes[i ].p0;
			// Get vector from focal point Origin to one of the triangle's corners (any of the three vertices, transformed in world)
			Vec3 p1 = triangles.vertices[triangles.indices[i * 3]];
			// Check if these are in opposite directions (this means the we can see the front triangle 
			if (p1 * sn < 0.0f)
			{
				triangles.cullFlags[i] = true;
			}
		}
		for (auto& v : triangles.vertices)
		{
			pst.Transform(v, true);
		}
		for (auto& a : triangles.normals_axes)
		{
			pst.Transform(a.p0, true);
			pst.Transform(a.p1, true);
		}
		// Draw triangles
		for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++)
		{
			if (triangles.cullFlags[i])
				gfx.DrawTriangle(
					triangles.vertices[triangles.indices[i * 3]],
					triangles.vertices[triangles.indices[i * 3 + 1]],
					triangles.vertices[triangles.indices[i * 3 + 2]],
					colors[3]);
		}
		// Draw Cube wireframe
		auto lines = pyramid.GetLines();
		size_t nLinePoints = lines.indices.size();
		for (size_t i = 0, end = nLinePoints - 2; i <= end; i += 2)
		{
			gfx.DrawLine(triangles.vertices[lines.indices[i]], triangles.vertices[lines.indices[i + 1]], Colors::White);
		}
		// Overdraw front facing edges
		for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++)
		{
			if (triangles.cullFlags[i])
			{
				gfx.DrawLine(triangles.vertices[triangles.indices[i * 3]], triangles.vertices[triangles.indices[i * 3 + 1]], Colors::Gray);
				gfx.DrawLine(triangles.vertices[triangles.indices[i * 3 + 1]], triangles.vertices[triangles.indices[i * 3 + 2]], Colors::Gray);
				gfx.DrawLine(triangles.vertices[triangles.indices[i * 3]], triangles.vertices[triangles.indices[i * 3 + 2]], Colors::Gray);
			}
		}
		// Draw axes & normals
		for (auto& a : triangles.normals_axes)
		{
			gfx.DrawLine({ a.p0.x, a.p0.y }, { a.p1.x, a.p1.y }, a.col);
		}
	}
private:
	PubeScreenTransformer pst;
	static constexpr float AngVel = (float)PI / 14;
	float Dtheta_x = 0.0f;
	float Dtheta_y = 0.0f;
	float Dtheta_z = 0.0f;
	float offset_z = 3.0f;
	Mat3 R_main = Mat3::RotationBirdEye();
	Pyramid pyramid = Pyramid(0.5f);
	const Color colors[12] =
	{
		Colors::White,
		Colors::Green,
		Colors::Gray,
		Colors::LightGray,
		Colors::Red ,
		Colors::Green,
		Colors::Blue ,
		Colors::Yellow,
		Colors::Cyan,
		Colors::Magenta,
		Colors::Blue,
		Colors::Red
	};
};