#pragma once

#include "AppState.h"
#include "MapMaker.h"

#include "v2.h"

class Game : public AppState
{
	MapMaker::Tile* map;
	intV2 size = intV2();
	unsigned int seed = 0;
	
	v2 camPos = v2();

public:

	Game();

	void Update(float deltaTime);
	void Draw();

private:

	void DrawLevel();


};