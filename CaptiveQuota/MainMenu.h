#pragma once

#include "AppState.h"
#include "AABB.h"

#include "raylib.h"

class MainMenu : public AppState
{
	enum class MenuState
	{
		Normal,
		Settings,
		Credits,
	};

	MenuState state = MenuState::Normal;

	int titleX;
	int titleY;

	AABB playBox;

	AABB creditsBox;

	Music music;


	void NormalUpdate();
	void NormalDraw();
	void CreditsUpdate();
	void CreditsDraw();

public:
	MainMenu();
	~MainMenu();

	void Update(float deltaTime);
	void Draw(float deltaTime);
};