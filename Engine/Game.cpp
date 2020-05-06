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
		Mat3::RotationX( theta_x ) *
		Mat3::RotationY( theta_y ) *
		Mat3::RotationZ( theta_z );
	
	//std::vector<Vec3> pubSpaceVerts;
	for( auto& v : triangles.vertices )
	{
		v *= rot;
		v += { 0.0f,0.0f,offset_z };
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
     		pst.Transform( v,true );
		}
		else
		{
			pst.Transform(v,false);
		}
		//pubSpaceVerts.push_back(v);
	}
	for (auto i = triangles.indices.cbegin(),
		 end = triangles.indices.cend();
		 i != end; std::advance(i, 3)  )
	{
		// Get triangle vertices 
		Vec3 p1 = triangles.vertices[*i];
		Vec3 p2 = triangles.vertices[*std::next(i)];
		Vec3 p3 = triangles.vertices[*std::next(i, 2)];
		Vec3 surfCentroid = (p3 + p1) / 2;

		// Calculate triange and cube centroids
		Vec3 triangleCentroid = (p1 + p2 + p3) / 3;
		Vec3 cubeCentroid = { 0.f,0.f,offset_z };
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			pst.Transform(cubeCentroid, true);
		}
		else
		{
			pst.Transform(cubeCentroid, false);
		}
		Vec3 surfaceNormal_T = surfCentroid - cubeCentroid;

		// Calculate direction from cube centroid to triangle centroid
		Vec3 CtoT = triangleCentroid - cubeCentroid; 
		// Calculate directions of (any) two sides of triangle
		Vec3 tla = p1 - p2;
		Vec3 tlb = p1 - p3;
		
		// Apply cross product to get surface normal and orient outward from cube
		Vec3 surfaceNormal = { 
			tla.y * tlb.z - tla.z * tlb.y,
			tla.z * tlb.x - tla.x * tlb.z, 
			tla.x * tlb.y - tla.y * tlb.x 
		};
		if (surfaceNormal * CtoT < 0) surfaceNormal = -surfaceNormal;
		
		// Only draw triangle if outward surface normal points to camera
		if (surfaceNormal.z <0.0001 )
		//if (surfN.z < 0)
		{
			gfx.DrawTriangle( p1,p2,p3, colors[std::distance(triangles.indices.cbegin(), i) / 3] );
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
}