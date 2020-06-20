#pragma once

#include "Scene.h"
#include "PrimitiveTriangle.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "SolidEffect.h"

// scene demonstrating solid colored cube
class SceneTwoTriangles : public Scene
{
public:
	typedef Pipeline<SolidEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	SceneTwoTriangles(Graphics& gfx)
		:
		itlist(PrimitiveTriangle::GetPlain<Vertex>()),
		pipeline(gfx),
		Scene(L"Two triangles intersecting")
	{
		itlist.sides.clear();
		//Color colarr[8] = { Colors::Red, Colors::Green, Colors::Blue, Colors::Yellow, Colors::Cyan, Colors::Magenta, Colors::White };
		Color colarr[6] = { Colors::LightGray, Colors::Gray, Colors::LightGray, Colors::Gray, Colors::LightGray,Colors::Gray };
		for (size_t i = 0; i < 6; i++)
		{
			itlist.vertices[i].color = colarr[i / 3];
		}
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
		Mat3 rot_fixed = Mat3::RotationX(-Dtheta_x) * Mat3::RotationZ(-Dtheta_z) * Mat3::RotationY(-Dtheta_y);
		R_main_fixed = rot_fixed * R_main_fixed;
	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();
		// Draw Fixed Triangle
		
		// set pipeline transform
		//pipeline.BindRotation(R_main_fixed);
		pipeline.BindRotation(Mat3::Identity());
		pipeline.BindTranslation({ 0.0f,0.0f,2.0f });
		// render triangles
		auto VSet1 = pipeline.Draw(itlist);
		
		// Draw Mobile Triangle
		
		// set pipeline transform
		pipeline.BindRotation(R_main);
		pipeline.BindTranslation({ 0.0f,0.0f,offset_z });
		// render triangles
		auto VSet2 = pipeline.Draw(itlist);
		
		auto Vpair1 = pipeline.TrianglesIntersect(VSet1[0], VSet1[1], VSet1[2], VSet2[0], VSet2[1], VSet2[2]);
		auto Vpair2 = pipeline.TrianglesIntersect(VSet2[0], VSet2[1], VSet2[2], VSet1[0], VSet1[1], VSet1[2]);
		pipeline.DrawLine(Vpair1.second, Vpair1.first,Colors::Red);
		pipeline.DrawLine(Vpair2.second, Vpair2.first, Colors::Yellow);
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
	static constexpr float zVel = 0.1f;
	float offset_z = 2.0f;
	float Dtheta_x = 0.0f;
	float Dtheta_y = 0.0f;
	float Dtheta_z = 0.0f;
	Mat3 R_main = Mat3::RotationBirdEye();
	Mat3 R_main_fixed = Mat3::Identity();

};