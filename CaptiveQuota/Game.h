#pragma once

#include "AppState.h"
#include "Map.h"
#include "Player.h"

#include "v2.h"

class Game : public AppState
{
	Map map;
	
	v2 camPos = v2();

	Player player;

public:

	Game();

	void Update(float deltaTime);
	void Draw();

};