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
#include "SceneCubeSkin.h"


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	//scenes.push_back(std::make_unique<SolidCubeScene>());
	//scenes.push_back(std::make_unique<SolidPyramidScene>());
	//scenes.push_back(std::make_unique<TexSkinCubeScene>());
	scenes.push_back(std::make_unique<SceneCubeSkin>(gfx, L"Images\\ArrowL.bmp"));
	currentScene = scenes.begin();
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
	const float dt = ft.Mark();
	while (!wnd.kbd.KeyIsEmpty())
	{
		const auto e = wnd.kbd.ReadKey();
		if (e.GetCode() == VK_TAB && e.IsPress())
		{
			CycleScenes();
		}
	}
	// update scene
	(*currentScene)->Update(wnd.kbd, wnd.mouse, dt);
}

void Game::CycleScenes()
{
	if (std::next(currentScene) == scenes.end())
	{
		currentScene = scenes.begin();
	}
	else
	{
		currentScene = std::next(currentScene);
	}
}

	
void Game::ComposeFrame()
{
	(*currentScene)->Draw();
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
