#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"

// scene demonstrating skinned cube
class SceneCubeSkin : public Scene
{
public:
	typedef Pipeline::Vertex Vertex;
public:
	SceneCubeSkin(Graphics& gfx, Color c)
		:
		itlist(Cube::GetSkinned<Vertex>()),
		pipeline(gfx),
		Scene("Solid color Cube")
	{
		pipeline.BindTexture(c);
	}
	SceneCubeSkin(Graphics& gfx, const std::wstring& filename)
		:
		itlist(Cube::GetSkinned<Vertex>()),
		pipeline(gfx),
		Scene("Textured Cube skinned using texture: " + std::string(filename.begin(), filename.end()))
	{
		pipeline.BindTexture(filename);
	}
	virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		Dtheta_x = 0.0f;
		Dtheta_y = 0.0f;
		Dtheta_z = 0.0f;
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
		if (kbd.KeyIsPressed(VK_SPACE))
		{
			R_main = Mat3::Identity();
		}
		// Update rotation matrix based on keyboard input
		Mat3 rot = Mat3::RotationX(Dtheta_x) * Mat3::RotationZ(Dtheta_z) * Mat3::RotationY(Dtheta_y);
		R_main = rot * R_main;
	}
	virtual void Draw() override
	{
		// set pipeline transform
		pipeline.BindRotation(R_main);
		pipeline.BindTranslation({ 0.0f,0.0f,offset_z });
		// render triangles
		pipeline.Draw(itlist);
	}
private:
	IndexedTriangleList<Vertex> itlist;  // { Vector<Vertex> verts, Vector<size_t> indices, Vector<Axis> normal_axes} verts: in object space (from definition)
	Pipeline pipeline;
	static constexpr float angVel = PI/2;
	static constexpr float zVel = 2.0f;
	float offset_z = 5.0f;
	float Dtheta_x = 0.0f;
	float Dtheta_y = 0.0f;
	float Dtheta_z = 0.0f;
	Mat3 R_main = Mat3::RotationBirdEye();
};