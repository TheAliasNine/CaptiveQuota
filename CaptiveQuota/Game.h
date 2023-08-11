#pragma once

#include "AppState.h"
#include "Map.h"
#include "Player.h"

#include "v2.h"

#include "raylib.h"

class Game : public AppState
{
	Map map;
	
	v2 camPos = v2();

	Player player;
	
	Texture2D background;

	void PhysicStep();

public:

	Game();
	~Game();

	void Update(float deltaTime);
	void Draw();

};