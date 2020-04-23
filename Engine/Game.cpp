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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	cube(0.5f)
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
}

void Game::ComposeFrame()
{
	auto linelist = cube.GetLines();
	
	// Transform to screenspace
	for (auto& v : linelist.vertices) pst.Transform(v);
	
	// Draw all vertices based on indexlist
	for (auto it = linelist.indices.cbegin(),
		end = linelist.indices.cend();
		it != end;
		std::advance(it, 2))
	{
		gfx.DrawLine(linelist.vertices[*it], linelist.vertices[*std::next(it)], Colors::Red);
	}
	/* Old fashioned way
	for (int i = 0; i < linelist.indices.size() - 1; i+=2)
	{
		size_t i1 = linelist.indices[i];
		size_t i2 = linelist.indices[i+1];
		Vec3 v1 = linelist.vertices[i1];
		Vec3 v2 = linelist.vertices[i2];
		gfx.DrawLine(v1,v2, Colors::Red);
	}*/

}