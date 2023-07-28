#include "App.h"

#include "raylib.h"


App::App()
{

}

App::~App()
{
	if (running) Close();
}

void App::Init()
{
	running = true;
	InitWindow(WINDOWX, WINDOWY, "Captive Quota");
}


void App::Update()
{

}


void App::Draw()
{

}



void App::Close()
{
	running = false;
}