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

	creditsBox.size.x = MeasureText("Credits", 40);
	creditsBox.size.y = 40;
	creditsBox.position.x = WINDOWX / 2;
	creditsBox.position.y = WINDOWY - 110;
}

MainMenu::~MainMenu()
{
	StopMusicStream(music);
	UnloadMusicStream(music);
}

void MainMenu::Update(float deltaTime)
{
	UpdateMusicStream(music);
	switch (state)
	{
	case MenuState::Normal:
		NormalUpdate();
		break;

	case MenuState::Credits:
		CreditsUpdate();
		break;
	}
}

void MainMenu::Draw(float deltaTime)
{
	ClearBackground(DARKGRAY);
	switch (state)
	{
	case MenuState::Normal:
		NormalDraw();
		break;

	case MenuState::Credits:
		CreditsDraw();
		break;
	}
}

void MainMenu::NormalDraw()
{
	DrawText("Captive Quota", titleX, titleY, 60, BLACK);
	
	DrawText("Play Game", playBox.position.x - (playBox.size.x / 2), playBox.position.y - 40 / 2, 40, BLACK);
	
	DrawText("Credits", creditsBox.position.x - (creditsBox.size.x / 2), creditsBox.position.y - 40 / 2, 40, BLACK);
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
	else if (creditsBox.CheckCollision(mousePos))
	{
		state = MenuState::Credits;
	}
}

void MainMenu::CreditsDraw()
{
	int creditsSize = MeasureText("Credits", 60);
	DrawText("Credits", (WINDOWX - creditsSize) / 2, titleY, 60, BLACK);

	int backSize = MeasureText("Back", 40);
	DrawText("Back", creditsBox.position.x - (backSize / 2), creditsBox.position.y - 40 / 2, 40, BLACK);

	DrawText("Programming by Ethan Hudson", 50, 115, 20, BLACK);
	DrawText("Player Art by Trinity Cripps", 50, 140, 20, BLACK);
	DrawText("Menu music by brandon75689", 50, 165, 20, BLACK);
	DrawText("App starting sound by Lokif", 50, 190, 20, BLACK);

}

void MainMenu::CreditsUpdate()
{
	if (!InputManager::KeyPressed(InputManager::Confirm))
		return;
	v2 mousePos = v2(GetMousePosition().x, GetMousePosition().y);

	if (creditsBox.CheckCollision(mousePos))
	{
		state = MenuState::Normal;
	}
}