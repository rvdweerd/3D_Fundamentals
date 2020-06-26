#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "SolidEffect.h"

// scene demonstrating solid colored cube
class SceneDoubleCube : public Scene
{
public:
	typedef Pipeline<SolidEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	SceneDoubleCube(Graphics& gfx)
		:
		itlist(Cube::GetIndependentFaces<Vertex>()),
		pipeline(gfx),
		Scene(L"Vertex color Cube")
	{
		itlist.sides.clear();
		//Color colarr[8] = { Colors::Red, Colors::Green, Colors::Blue, Colors::Yellow, Colors::Cyan, Colors::Magenta, Colors::White, Colors::Black };
		Color colarr[6] = { Colors::White, Colors::White, Colors::White, Colors::White, Colors::White, Colors::White };
		for (size_t i = 0; i < 24; i++)
		{
			itlist.vertices[i].color = colarr[i/4];
		}
	}
	virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		Dtheta_x = 0.0f;
		Dtheta_y = 0.0f;
		Dtheta_z = 0.0f;
		Dtheta_x2 = 0.0f;
		Dtheta_y2 = 0.0f;
		Dtheta_z2 = 0.0f;
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
		if (kbd.KeyIsPressed('T'))
		{
			Dtheta_x2 = angVel * dt;
		}
		if (kbd.KeyIsPressed('Y'))
		{
			Dtheta_y2 = angVel * dt;
		}
		if (kbd.KeyIsPressed('U'))
		{
			Dtheta_z2 = angVel * dt;
		}
		if (kbd.KeyIsPressed('G'))
		{
			Dtheta_x2 = -angVel * dt;
		}
		if (kbd.KeyIsPressed('H'))
		{
			Dtheta_y2 = -angVel * dt;
		}
		if (kbd.KeyIsPressed('J'))
		{
			Dtheta_z2 = -angVel * dt;
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
		// Update rotation matrix based on keyboard input
		Mat3 rot = Mat3::RotationX(Dtheta_x) * Mat3::RotationZ(Dtheta_z) * Mat3::RotationY(Dtheta_y);
		R_main = rot * R_main;
		Mat3 rot2 = Mat3::RotationX(Dtheta_x2) * Mat3::RotationZ(Dtheta_z2) * Mat3::RotationY(Dtheta_y2);
		R_main2 = rot2  * R_main2;
	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();
		// Draw Fixed Cube
		// set pipeline transform
		pipeline.BindRotation(R_main2);
		pipeline.BindTranslation({ 0.0f,0.0f,2.0f });
		// render triangles
		std::vector<Vertex> VSet1 = pipeline.Draw(itlist);
		
		// Draw Mobile Cube	
		// set pipeline transform
		pipeline.BindRotation(R_main);
		pipeline.BindTranslation({ 0.0f,0.0f,offset_z });
		// render triangles
		std::vector<Vertex> VSet2 = pipeline.Draw(itlist);
		
		pipeline.DrawTriangleEdges(itlist);
		
		pipeline.BindRotation(R_main2);
		pipeline.BindTranslation({ 0.0f,0.0f,2.0f });
		pipeline.DrawTriangleEdges(itlist);
	}
	virtual float GetAngVel() override
	{
		return angVel;
	}
public:
	float angVel = PI / 3;
private:
	IndexedTriangleList<Vertex> itlist;  // { Vector<Vertex> verts, Vector<size_t> indices, Vector<Axis> normal_axes} verts: in object space (from definition)
	Pipeline pipeline;
	static constexpr float zVel = 1.0f;
	float offset_z = 2.0f;
	float Dtheta_x = 0.0f;
	float Dtheta_y = 0.0f;
	float Dtheta_z = 0.0f;
	float Dtheta_x2 = 0.0f;
	float Dtheta_y2 = 0.0f;
	float Dtheta_z2 = 0.0f;
	Mat3 R_main = Mat3::RotationBirdEye();
	Mat3 R_main2 = Mat3::Identity();

};