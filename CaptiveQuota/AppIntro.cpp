#include "AppIntro.h"

#include "MainMenu.h"

AppIntro::AppIntro()
{
	timer = lengthTime;
	wave = LoadWave("Assets\\Sound\\AppIntro.wav");
	sound = LoadSoundFromWave(wave);
}

void AppIntro::Draw(float deltaTime)
{
	ClearBackground(DARKGRAY);
	int fontSize = 40;
	int textLength = 20;
	char name[] = "Made by Ethan Hudson";
	DrawText(name, (WINDOWX - MeasureText(name, fontSize)) / 2, WINDOWY / 2 - fontSize / 2, fontSize, BLACK);
}


void AppIntro::Update(float deltaTime)
{
	if (!soundPlayed && timer < lengthTime - soundDelay)
	{
		soundPlayed = true;
		PlaySound(sound);
	}
	timer -= deltaTime;
	if (timer <= 0)
	{
		UnloadWave(wave);
		UnloadSound(sound);
		closed = true;
		nextState = new MainMenu();
	}
}