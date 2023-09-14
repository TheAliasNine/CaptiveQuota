#pragma once

#include "AppState.h"
#include "AABB.h"

#include "raylib.h"

class MainMenu : public AppState
{
	int titleX;
	int titleY;

	AABB playBox;

	int creditsX = 375;
	int creditsY = 500;

	Music music;


	void NormalUpdate();
	void NormalDraw();

public:
	MainMenu();
	~MainMenu();

	void Update(float deltaTime);
	void Draw(float deltaTime);
};