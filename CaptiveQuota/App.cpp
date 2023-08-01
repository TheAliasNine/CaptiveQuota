#include "App.h"

#include "raylib.h"


#include "AppIntro.h"

App::App()
{

}

App::~App()
{
	Close();
}

void App::Init()
{
	lastTime = std::chrono::steady_clock::now();
	running = true;
	InitWindow(WINDOWX, WINDOWY, "Captive Quota");
	InitAudioDevice();
	SetExitKey(0);

	state = new AppIntro();
}


void App::Update()
{
	if (WindowShouldClose())
	{
		running = false;
		return;
	}
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = now - lastTime;
	lastTime = now;

	state->Update(deltaTime.count());
	if (state->Closed())
	{
		AppState* nextState = state->NextState();
		delete state;
		state = nextState;
		if (state == nullptr) Close();
	}
}


void App::Draw()
{
	if (!running) return;
	BeginDrawing();
	state->Draw();
	EndDrawing();
}



void App::Close()
{
	if (!running) return;
	CloseWindow();
	CloseAudioDevice();
	running = false;

	if (state != nullptr)
	{
		delete state;
	}
}