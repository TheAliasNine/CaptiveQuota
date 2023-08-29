#pragma once

#include "AppState.h"

#include "raylib.h"

class AppIntro : public AppState
{
	const float lengthTime = 2.6f;
	const float soundDelay = 0.8f;
	bool soundPlayed = false;
	float timer;

	Wave wave;
	Sound sound;
public:

	AppIntro();

	void Draw(float deltaTime);

	void Update(float deltaTime);


};