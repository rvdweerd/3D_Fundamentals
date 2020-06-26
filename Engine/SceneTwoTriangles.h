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
		itlist1(PrimitiveTriangle::GetPlain<Vertex>()),
		itlist2(PrimitiveTriangle::GetPlain<Vertex>()),
		pipeline(gfx),
		Scene(L"Two triangles intersecting")
	{
		itlist1.sides.clear(); itlist2.sides.clear();
		//Color colarr[8] = { Colors::Red, Colors::Green, Colors::Blue, Colors::Yellow, Colors::Cyan, Colors::Magenta, Colors::White };
		Color colarr[6] = { Colors::LightGray, Colors::Gray, Colors::LightGray, Colors::Gray, Colors::LightGray,Colors::Gray };
		for (size_t i = 0; i < 6; i++)
		{
			itlist1.vertices[i].color = Colors::White;//colarr[i / 3];
			itlist2.vertices[i].color = Colors::LightGray;//colarr[i / 3];
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
		R_main2 = rot2 * R_main2;
	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();
		// Draw Fixed Triangle
		
		// set pipeline transform
		//pipeline.BindRotation(R_main_fixed);
		pipeline.BindRotation(R_main2);
		pipeline.BindTranslation({ 0.0f,0.0f,1.3f });
		// render triangles
		std::vector<Vertex> VSet1 = pipeline.Draw(itlist1);
		
		// Draw Mobile Triangle
		
		// set pipeline transform
		pipeline.BindRotation(R_main);
		pipeline.BindTranslation({ 0.0f,0.0f,offset_z });
		// render triangles
		
		std::vector<Vertex> VSet2 = pipeline.Draw(itlist2);
		
		{/*
			IntersectData iDat1 = pipeline.TrianglesIntersect(VSet1[0], VSet1[1], VSet1[2], VSet2[0], VSet2[1], VSet2[2]);
			IntersectData iDat2 = pipeline.TrianglesIntersect(VSet2[0], VSet2[1], VSet2[2], VSet1[0], VSet1[1], VSet1[2]);

			Vec3 T1 = (iDat1.Verts_intersect.first - iDat1.O);
			Vec3 T2 = (iDat1.Verts_intersect.second - iDat1.O);
			Vec3 Q1 = (iDat2.Verts_intersect.first - iDat1.O);
			Vec3 Q2 = (iDat2.Verts_intersect.second - iDat1.O);
			float t1, t2, q1, q2;

			if (abs(iDat1.D.x) >= abs(iDat1.D.y) && abs(iDat1.D.x) >= abs(iDat1.D.z)) t1 = T1.x / iDat1.D.x;
			else if (abs(iDat1.D.y) >= abs(iDat1.D.x) && abs(iDat1.D.y) >= abs(iDat1.D.z)) t1 = T1.y / iDat1.D.y;
			else t1 = T1.z / iDat1.D.z;

			if (abs(iDat1.D.x) >= abs(iDat1.D.y) && abs(iDat1.D.x) >= abs(iDat1.D.z)) t2 = T2.x / iDat1.D.x;
			else if (abs(iDat1.D.y) >= abs(iDat1.D.x) && abs(iDat1.D.y) >= abs(iDat1.D.z)) t2 = T2.y / iDat1.D.y;
			else t2 = T2.z / iDat1.D.z;

			if (abs(iDat1.D.x) >= abs(iDat1.D.y) && abs(iDat1.D.x) >= abs(iDat1.D.z)) q1 = Q1.x / iDat1.D.x;
			else if (abs(iDat1.D.y) >= abs(iDat1.D.x) && abs(iDat1.D.y) >= abs(iDat1.D.z)) q1 = Q1.y / iDat1.D.y;
			else q1 = Q1.z / iDat1.D.z;

			if (abs(iDat1.D.x) >= abs(iDat1.D.y) && abs(iDat1.D.x) >= abs(iDat1.D.z)) q2 = Q2.x / iDat1.D.x;
			else if (abs(iDat1.D.y) >= abs(iDat1.D.x) && abs(iDat1.D.y) >= abs(iDat1.D.z)) q2 = Q2.y / iDat1.D.y;
			else q2 = Q2.z / iDat1.D.z;

			if (t1 > t2)
			{
				std::swap(t1, t2);
				std::swap(T1, T2);
			}
			if (q1 > q2)
			{
				std::swap(q1, q2);
				std::swap(Q1, Q2);
			}
			Color c = Colors::Red;
			if (t1 >= q1 && q2 >= t1 && t2 >= q2) pipeline.DrawLine(T1 + iDat1.O, Q2 + iDat1.O, c);
			else if (t1 >= q1 && t2 <= q2) pipeline.DrawLine(T1 + iDat1.O, T2 + iDat1.O, c);
			else if (q1 >= t1 && q2 <= t2) pipeline.DrawLine(Q1 + iDat1.O, Q2 + iDat1.O, c);
			else if (q1 >= t1 && q1 <= t2 && q2 >= t2) pipeline.DrawLine(Q1 + iDat1.O, T2 + iDat1.O, c);
			*/
		}
	}
	virtual float GetAngVel() override
	{
		return angVel;
	}
public:
	float angVel = PI / 3;
private:
	IndexedTriangleList<Vertex> itlist1;  // { Vector<Vertex> verts, Vector<size_t> indices, Vector<Axis> normal_axes} verts: in object space (from definition)
	IndexedTriangleList<Vertex> itlist2;
	Pipeline pipeline;
	static constexpr float zVel = 0.1f;
	float offset_z = 1.1f;
	float Dtheta_x = 0.0f;
	float Dtheta_y = 0.0f;
	float Dtheta_z = 0.0f;
	float Dtheta_x2 = 0.0f;
	float Dtheta_y2 = 0.0f;
	float Dtheta_z2 = 0.0f;
	Mat3 R_main = Mat3::RotationBirdEye();
	Mat3 R_main2 = Mat3::Identity();

};