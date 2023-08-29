#pragma once

#include "AppState.h"

#include <chrono>

class App
{
	bool running = false;

	AppState* state = nullptr;

	float m_deltaTime;

	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

public:
	App();
	~App();
	//No more rule of 5, app should never be copied

	void Init();
	
	void Update();
	void Draw();
	
	void Close();

	bool Running() { return running; }
};