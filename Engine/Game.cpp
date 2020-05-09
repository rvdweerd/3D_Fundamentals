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
	const float dt = 2.0f / 60.0f;
	if( wnd.kbd.KeyIsPressed( 'Q' ) )
	{
		theta_x = wrap_angle( theta_x + dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'W' ) )
	{
		theta_y = wrap_angle( theta_y + dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'E' ) )
	{
		theta_z = wrap_angle( theta_z + dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'A' ) )
	{
		theta_x = wrap_angle( theta_x - dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'S' ) )
	{
		theta_y = wrap_angle( theta_y - dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'D' ) )
	{
		theta_z = wrap_angle( theta_z - dTheta * dt );
	}
	if (wnd.kbd.KeyIsPressed('R'))
	{
		offset_z += 2.0f*dt;
	}
	if (wnd.kbd.KeyIsPressed('F'))
	{
		offset_z -= 2.0f * dt;
	}
}

void Game::ComposeFrame()
{
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
	//auto lines = cube.GetLines();
	auto triangles = cube.GetTriangles();
	const Mat3 rot =
		Mat3::RotationX(theta_x) *
		Mat3::RotationY(theta_y) *
		Mat3::RotationZ(theta_z);

	//std::vector<Vec3> pubSpaceVerts;
	for (auto& v : triangles.vertices)
	{
		v *= rot;
		v += { 0.0f, 0.0f, offset_z};// offset_z
	}
	for (auto& v : triangles.normals_axes)
	{
		v.p0 *= rot;
		v.p1 *= rot;
		v.p0 += { 0.0f, 0.0f, offset_z};// offset_z
		v.p1 += { 0.0f, 0.0f, offset_z};// offset_z
	}
	for (size_t i = 0; i < triangles.indices.size() / 3; i++)
	{
		// Get triangle vertices 
		Vec3 p1 = triangles.vertices[ triangles.indices[i * 3] ];
		Vec3 p2 = triangles.vertices[ triangles.indices[i * 3 + 1]];
		Vec3 p3 = triangles.vertices[ triangles.indices[i * 3 + 2]];
		// Apply cross product to get surface normal and orient outward from cube
		//Vec3 surfCentroid = (p3 + p1) / 2;
		Vec3 surfaceNormal = (p1 - p2) % (p1 - p3);
		if (p1 * surfaceNormal < 0.0f)
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
	for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++)
	{
		if (triangles.cullFlags[i])
		gfx.DrawTriangle(triangles.vertices[triangles.indices[i * 3]], triangles.vertices[triangles.indices[i * 3 + 1]], triangles.vertices[triangles.indices[i * 3 + 2]],
			colors[i]);
	}
	for (auto& a : triangles.normals_axes)
	{
		gfx.DrawLine({ a.p0.x, a.p0.y }, { a.p1.x, a.p1.y }, a.col);
	}
		// Only draw triangle if outward surface normal points to camera
		//if (!wnd.kbd.KeyIsPressed(VK_SPACE))
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
