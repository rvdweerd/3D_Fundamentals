#pragma once
#include "Scene.h"
#include "Mat3.h"
#include "PubeScreenTransformer.h"
#include "CubeWithSkin.h"

class TexSkinCubeScene : public Scene
{
public:
	TexSkinCubeScene() = default;
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
		auto triangles = cube.GetTrianglesTex();

		//std::vector<Vec3> pubSpaceVerts;
		for (auto& v : triangles.vertices)
		{
			v.pos *= R_main;
			v.pos += { 0.0f, 0.0f, offset_z};// offset_z
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
			Vec3 sn = triangles.normals_axes[i / 2].p1 - triangles.normals_axes[i / 2].p0;
			// Get vector from focal point Origin to one of the triangle's corners (any of the three vertices, transformed in world)
			Vec3 p1 = triangles.vertices[triangles.indices[i * 3]].pos;
			// Check if these are in opposite directions (this means the we can see the front triangle 
			if (p1 * sn < 0.0f)
			{
				triangles.cullFlags[i] = true;
			}
		}
		for (auto& v : triangles.vertices)
		{
			pst.Transform(v.pos, true);
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
				gfx.DrawTriangleTex(
					triangles.vertices[triangles.indices[i * 3]],
					triangles.vertices[triangles.indices[i * 3 + 1]],
					triangles.vertices[triangles.indices[i * 3 + 2]],
					sbTex);
		}
		// Draw Cube wireframe
		auto lines = cube.GetLines();
		size_t nLinePoints = lines.indices.size();
		for (size_t i = 0, end = nLinePoints - 2; i <= end; i += 2)
		{
			//gfx.DrawLine(triangles.vertices[lines.indices[i]].pos, triangles.vertices[lines.indices[i + 1]].pos, Colors::White);
		}
		// Overdraw front facing edges
		for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++)
		{
			if (triangles.cullFlags[i])
			{
				gfx.DrawLine(triangles.vertices[triangles.indices[i * 3]].pos, triangles.vertices[triangles.indices[i * 3 + 1]].pos, Colors::Gray);
				gfx.DrawLine(triangles.vertices[triangles.indices[i * 3 + 1]].pos, triangles.vertices[triangles.indices[i * 3 + 2]].pos, Colors::Gray);
				gfx.DrawLine(triangles.vertices[triangles.indices[i * 3]].pos, triangles.vertices[triangles.indices[i * 3 + 2]].pos, Colors::Gray);
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
	static constexpr float AngVel = (float)PI / 2;// rotation speed in radians/second
	float Dtheta_x = 0.0f;
	float Dtheta_y = 0.0f;
	float Dtheta_z = 0.0f;
	float offset_z = 5.0f;
	Mat3 R_main = Mat3::RotationBirdEye();
	CubeWithSkin cube = CubeWithSkin(1.0f);

	//Surface sbTex = Surface::FromFile(L"Images\\eye.bmp");
	//Surface sbTex = Surface::FromFile(L"Images\\link90x90.bmp");
	//Surface sbTex = Surface::FromFile(L"Images\\skin.bmp");
	//Surface sbTex = Surface::FromFile(L"Images\\dice.bmp");
	Surface sbTex = Surface::FromFile(L"Images\\wood24.bmp");
};