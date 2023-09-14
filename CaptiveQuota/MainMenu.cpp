#include "MainMenu.h"

#include "Game.h"
#include "InputManager.h"
#include "v2.h"

#include "raylib.h"


MainMenu::MainMenu()
{
	music = LoadMusicStream("Assets\\Sound\\MenuMusic.ogg");
	PlayMusicStream(music);

	InputManager::EnableBind(InputManager::Confirm);

	titleX = (WINDOWX - MeasureText("Captive Quota", 60)) / 2;
	titleY = 60;

	playBox.size.x = MeasureText("Play Game", 40);
	playBox.size.y = 40;
	playBox.position.x = WINDOWX / 2;
	playBox.position.y = 180;
}

MainMenu::~MainMenu()
{
	StopMusicStream(music);
	UnloadMusicStream(music);
}

void MainMenu::Update(float deltaTime)
{
	UpdateMusicStream(music);
	NormalUpdate();
}

void MainMenu::Draw(float deltaTime)
{
	ClearBackground(DARKGRAY);
	NormalDraw();
}

void MainMenu::NormalDraw()
{
	DrawText("Captive Quota", titleX, titleY, 60, BLACK);
	
	DrawText("Play Game", playBox.position.x - (playBox.size.x / 2), playBox.position.y - 40 / 2, 40, BLACK);
	
	DrawText("Credits in AssetSources.txt", creditsX, creditsY, 30, BLACK);
}

void MainMenu::NormalUpdate()
{
	if (!InputManager::KeyPressed(InputManager::Confirm))
		return;
	v2 mousePos = v2(GetMousePosition().x, GetMousePosition().y);

	if (playBox.CheckCollision(mousePos))
	{
		closed = true;
		nextState = new Game();
	}
}