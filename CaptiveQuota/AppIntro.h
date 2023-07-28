#pragma once

#include "AppState.h"

#include "raylib.h"

class AppIntro : public AppState
{
	const float lengthTime = 1.5f;
	float timer;

public:

	AppIntro();

	void Draw();

	void Update(float deltaTime);


};