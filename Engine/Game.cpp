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
#include "SceneCubeVertexColor.h"
#include "SceneCubeSolidColors.h"
#include "ScenePyramidSkin.h"
#include "SceneRectangleSkin.h"
#include "SceneDoubleCube.h"
#include "SceneTwoTriangles.h"
#include "SceneVertexWave.h"
#include "SceneVertexWaveSolid.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	bmText(gfx)//,
	//zb(Graphics::ScreenWidth, Graphics::ScreenHeight)
{
	
	scenes.push_back(std::make_unique<SceneVertexWave>(gfx));
	scenes.push_back(std::make_unique<SceneVertexWaveSolid>(gfx));
	//scenes.push_back(std::make_unique<SceneTwoTriangles>(gfx));
	//scenes.push_back(std::make_unique<SceneDoubleCube>(gfx));
	//scenes.push_back(std::make_unique<SceneCubeSkin>(gfx, L"Images\\skin.bmp"));
	//scenes.push_back(std::make_unique<SceneCubeSkin>(gfx, L"Images\\wood256.bmp"));
	//scenes.push_back(std::make_unique<SceneRectangleSkin>(gfx, L"Images\\skin_pyr.bmp"));
	//scenes.push_back(std::make_unique<ScenePyramidSkin>(gfx, L"Images\\skin_pyr.bmp"));
	//scenes.push_back(std::make_unique<ScenePyramidSkin>(gfx, L"Images\\wood256.bmp"));
	//scenes.push_back(std::make_unique<SceneCubeSkin>(gfx, Colors::LightGray));
	//scenes.push_back(std::make_unique<SceneCubeSkin>(gfx, L"Images\\link90x90.bmp"));
	//scenes.push_back(std::make_unique<SceneCubeSkin>(gfx, L"Images\\eye.bmp"));
	//scenes.push_back(std::make_unique<SceneCubeVertexColor>(gfx));

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
	bmText.DrawString(10, 10, Colors::Red, "FR: "+std::to_string(ft.GetFR()), BitmapText::Font::Consolas13x24);
	bmText.DrawString(10, 38, Colors::Red, "dPhi/dt (deg/s): " + std::to_string((*currentScene)->GetAngVel()*180/PI), BitmapText::Font::Consolas13x24);

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
