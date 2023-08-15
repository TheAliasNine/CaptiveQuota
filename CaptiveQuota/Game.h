#pragma once

#include "AppState.h"
#include "Map.h"
#include "Minimap.h"
#include "Player.h"

#include "v2.h"

#include "raylib.h"

class Game : public AppState
{
	Map map;
	Minimap minimap;
	
	v2 camPos = v2();

	Player player;
	
	Texture2D background;

	void PhysicStep();

	void DrawUI();

public:

	Game();
	~Game();

	void Update(float deltaTime);
	void Draw();

};