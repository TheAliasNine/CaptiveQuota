#include "Game.h"

#include "InputManager.h"

#include "raylib.h"

Game::Game()
{
	map = Map();
	map.CreateMap(std::time(nullptr));
	map.SetCellSize(100);

	camPos = v2(map.PlayerSpawn().x * map.CellSize(), map.PlayerSpawn().y * map.CellSize());
	player = Player();
	player.position = v2(map.PlayerSpawn().x, map.PlayerSpawn().y) * map.CellSize();

	InputManager::EnableBind(InputManager::Up);
	InputManager::EnableBind(InputManager::Down);
	InputManager::EnableBind(InputManager::Left);
	InputManager::EnableBind(InputManager::Right);
}


void Game::Update(float deltaTime)
{
	player.Update(deltaTime);
	camPos = player.position - v2(WINDOWX / 2, WINDOWY / 2);
	
}

void Game::Draw()
{
	ClearBackground(Color{17, 14, 12, 255});
	
	map.DrawTiles(camPos);
	player.Draw();
}