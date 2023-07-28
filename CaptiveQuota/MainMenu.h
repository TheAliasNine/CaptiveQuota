#pragma once

#include "AppState.h"

class MainMenu : public AppState
{
public:
	void Update(float deltaTime);
	void Draw();
};