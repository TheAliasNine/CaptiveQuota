#pragma once

#include "AppState.h"

class Game : public AppState
{

public:

	Game();

	void Update(float deltaTime);
	void Draw();
};