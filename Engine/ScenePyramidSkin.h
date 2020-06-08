#pragma once

#include "Scene.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "TextureEffect.h"
#include "Pyramid.h"

// scene demonstrating skinned pyramid
class ScenePyramidSkin : public Scene
{
public:
	typedef Pipeline<TextureEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	ScenePyramidSkin(Graphics& gfx, Color c)
		:
		itlist(Pyramid::GetSkinned<Vertex>()),
		pipeline(gfx),
		Scene(L"Solid color Cube")
	{
		pipeline.effect.ps.BindTexture(c);
	}
	ScenePyramidSkin(Graphics& gfx, const std::wstring& filename)
		:
		itlist(Pyramid::GetSkinned<Vertex>()),
		pipeline(gfx),
		Scene(L"Textured Cube skinned using texture: " + std::wstring(filename.begin(), filename.end()))
	{
		pipeline.effect.ps.BindTexture(filename);
	}
	virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		Dtheta_x = 0.0f;
		Dtheta_y = 0.0f;
		Dtheta_z = 0.0f;
		Dtheta_r = 0.0f;
		Dtheta_r2 = 0.0f;

		if (kbd.KeyIsPressed('Q'))
		{
			Dtheta_x = angVel * dt;
		}
		if (kbd.KeyIsPressed('W'))
		{
			Dtheta_y = angVel * dt;
		}
		if (kbd.KeyIsPressed('E'))
		{
			Dtheta_z = angVel * dt;
		}
		if (kbd.KeyIsPressed('T'))
		{
			Dtheta_r = angVel * dt;
		}
		if (kbd.KeyIsPressed('Y'))
		{
			Dtheta_r2 = angVel * dt;
		}
		if (kbd.KeyIsPressed('A'))
		{
			Dtheta_x = -angVel * dt;
		}
		if (kbd.KeyIsPressed('S'))
		{
			Dtheta_y = -angVel * dt;
		}
		if (kbd.KeyIsPressed('D'))
		{
			Dtheta_z = -angVel * dt;
		}
		if (kbd.KeyIsPressed('R'))
		{
			offset_z += zVel * dt;
		}
		if (kbd.KeyIsPressed('F'))
		{
			offset_z -= zVel * dt;
		}
		if (kbd.KeyIsPressed('P'))
		{
			angVel += 1 * PI / 180;
		}
		if (kbd.KeyIsPressed('L'))
		{
			angVel -= 1 * PI / 180;
		}
		if (kbd.KeyIsPressed(VK_SPACE))
		{
			R_main = Mat3::Identity();
		}
		// Rotation around the first face normal (demo of rotation around arbitrary axis)
		if (Dtheta_r > 0.0f)
		{
			// Create new orthonormal basis whose x-axis aligns with the rotation vector r
			// get surface normal
			Vec3 r = itlist.normals_axes[0].p1;
			r.Normalize();
			// construct first perpendicular vector (method: Real-Time Rendering ed4, pp75; Cunningham, Steve, “3D Viewing and Rotation 
			// Using Orthonormal Bases,” in Andrew S. Glassner, ed., Graphics Gems, Academic Press, pp. 516–521, 1990
			// numerically stable way: find smallest (abs) component of r, set it to 0; swap & negate 2nd and 3rd components
			Vec3 s;
			if		(abs(r.x) <= abs(r.y) && abs(r.x) <= abs(r.z)) s = { 0.f,-r.z,r.y };
			else if (abs(r.y) <= abs(r.x) && abs(r.y) <= abs(r.z)) s = { -r.z,0.f,r.x };
			else if (abs(r.z) <= abs(r.x) && abs(r.z) <= abs(r.y)) s = { -r.y,r.x,0.f };
			s.Normalize();
			// construct second perpendicular vector (cross product)
			Vec3 t = r % s;
			t.Normalize();
			// Create the rotation matrix (consists of new basis vectors)
			Mat3 M = Mat3::ColMat(r, s, t);
			Mat3 M_T = M.Transpose();
			Mat3 rot_r1 = M * Mat3::RotationX(Dtheta_r) * M_T;
			R_main = rot_r1 * R_main;
		}
		if (Dtheta_r2 > 0.0f)
		{
			Vec3 r2 = itlist.normals_axes[1].p1;
			r2.Normalize();
			Vec3 s2;
			if (abs(r2.x) <= abs(r2.y) && abs(r2.x) <= abs(r2.z)) s2 = { 0.f,-r2.z,r2.y };
			else if (abs(r2.y) <= abs(r2.x) && abs(r2.y) <= abs(r2.z)) s2 = { -r2.z,0.f,r2.x };
			else if (abs(r2.z) <= abs(r2.x) && abs(r2.z) <= abs(r2.y)) s2 = { -r2.y,r2.x,0.f };
			s2.Normalize();
			Vec3 t2 = r2 % s2;
			t2.Normalize();
			Mat3 M2 = Mat3::ColMat(r2, s2, t2);
			Mat3 M_T2 = M2.Transpose();
			Mat3 rot_r2 = M2 * Mat3::RotationX(Dtheta_r2) * M_T2;
			R_main = rot_r2 * R_main;
		}

		// Update rotation matrix based on keyboard input
		Mat3 rot_xyz = Mat3::RotationX(Dtheta_x) * Mat3::RotationZ(Dtheta_z) * Mat3::RotationY(Dtheta_y);
		R_main = rot_xyz * R_main;
	}
	virtual void Draw() override
	{
		// set pipeline transform
		pipeline.BindRotation(R_main);
		pipeline.BindTranslation({ 0.0f,0.0f,offset_z });
		// render triangles
		pipeline.Draw(itlist);
	}
	virtual float GetAngVel() override
	{
		return angVel;
	}
private:
	IndexedTriangleList<Vertex> itlist;  // { Vector<Vertex> verts, Vector<size_t> indices, Vector<Axis> normal_axes} verts: in object space (from definition)
	Pipeline pipeline;
	float angVel = PI / 2;
	static constexpr float zVel = 2.0f;
	float offset_z = 5.0f;
	float Dtheta_x = 0.0f;
	float Dtheta_y = 0.0f;
	float Dtheta_z = 0.0f;
	float Dtheta_r = 0.0f;
	float Dtheta_r2 = 0.0f;
	Mat3 R_main = Mat3::RotationBirdEye();
};