/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Mat3.h"
#include <set>
Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	cube( 0.5f )
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = 4.0f / 60.0f;
	Dtheta_x = 0.0f;
	Dtheta_y = 0.0f;
	Dtheta_z = 0.0f;
	if (wnd.kbd.KeyIsPressed('Q'))
	{
		Dtheta_x = AngVel * dt;
	}
	if (wnd.kbd.KeyIsPressed('W'))
	{
		Dtheta_y = AngVel * dt;
	}
	if (wnd.kbd.KeyIsPressed('E'))
	{
		Dtheta_z = AngVel * dt;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		Dtheta_x = -AngVel * dt;
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		Dtheta_y = -AngVel * dt;
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		Dtheta_z = -AngVel * dt;
	}
	if (wnd.kbd.KeyIsPressed('R'))
	{
		offset_z += 0.5f * dt;
	}
	if (wnd.kbd.KeyIsPressed('F'))
	{
		offset_z -= 0.5f * dt;
	}
	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		R = Mat3::Identity();
		
	}
}

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
void Game::ComposeFrame()
{
	auto triangles = cube.GetTriangles();
	// get local axis orientation
	Vec3 x_local = (triangles.normals_axes[1].p1 - triangles.normals_axes[1].p0).GetNormalized();
	Vec3 y_local = (triangles.normals_axes[3].p1 - triangles.normals_axes[3].p0).GetNormalized();
	Vec3 z_local = (triangles.normals_axes[5].p1 - triangles.normals_axes[5].p0).GetNormalized();
	Mat3 Q = Mat3::ColMat(x_local, y_local, z_local);
	Mat3 QT = Q.Transpose();

	//auto lines = cube.GetLines();
	const Mat3 rot =
		Mat3::RotationX(Dtheta_x ) *
		Mat3::RotationZ(Dtheta_z) *
		Mat3::RotationY(Dtheta_y ) 
		;
	R = rot*R;

	//std::vector<Vec3> pubSpaceVerts;
	for (auto& v : triangles.vertices)
	{
		v *= R;
		v += { 0.0f, 0.0f, offset_z};// offset_z
	}
	for (auto& a : triangles.normals_axes)
	{
		a.p0 *= R;
		a.p1 *= R;
		a.p0 += { 0.0f, 0.0f, offset_z};// offset_z
		a.p1 += { 0.0f, 0.0f, offset_z};// offset_z
	}
	// Apply backface culling
	for (size_t i = 0; i < triangles.indices.size() / 3; i++)
	{
		// Get transformed surface normal (brought to focal point Origin)
		Vec3 sn = triangles.normals_axes[i/2].p1 - triangles.normals_axes[i/2].p0;
		// Get vector from focal point Origin to one of the triangle's corners (any of the three vertices, transformed in world)
		Vec3 p1 = triangles.vertices[ triangles.indices[i * 3] ];
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
	auto lines = cube.GetLines();
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
			gfx.DrawLine(triangles.vertices[triangles.indices[i * 3]],triangles.vertices[triangles.indices[i * 3 + 1]],Colors::Gray);
			gfx.DrawLine(triangles.vertices[triangles.indices[i * 3 + 1]],triangles.vertices[triangles.indices[i * 3 + 2]],Colors::Gray);
		}
	}
	// Draw axes & normals
	for (auto& a : triangles.normals_axes)
	{
		gfx.DrawLine({ a.p0.x, a.p0.y }, { a.p1.x, a.p1.y }, a.col);
	}
}
			

		

	//auto lines = teapot.GetLines();
	//const Mat3 rot =
	//	Mat3::RotationX(theta_x) *
	//	Mat3::RotationY(theta_y) *
	//	Mat3::RotationZ(theta_z);
	//for (auto& v : lines.vertices)
	//{
	//	v *= rot;
	//	v += { 0.0f, 0.0f, offset_z };
	//	pst.Transform(v);
	//}
	//for (auto i = lines.indices.cbegin(),
	//	end = lines.indices.cend();
	//	i != end; std::advance(i, 2))
	//{
	//	gfx.DrawLine(lines.vertices[*i-1], lines.vertices[*std::next(i)-1], Colors::Red);
	//}
