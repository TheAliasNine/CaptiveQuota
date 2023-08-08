#include "Game.h"

#include "InputManager.h"

#include "raylib.h"

Game::Game()
{
	seed = std::time(nullptr);

	MapMaker mapMaker = MapMaker(seed);
	map = mapMaker.MakeMap();
	size = mapMaker.size;

	InputManager::EnableBind(InputManager::Up);
	InputManager::EnableBind(InputManager::Down);
	InputManager::EnableBind(InputManager::Left);
	InputManager::EnableBind(InputManager::Right);
}


void Game::Update(float deltaTime)
{
	float speed = 200;
	if (InputManager::KeyDown(InputManager::Up))
	{
		camPos.y -= speed * deltaTime;
	}
	if (InputManager::KeyDown(InputManager::Down))
	{
		camPos.y += speed * deltaTime;
	}
	if (InputManager::KeyDown(InputManager::Left))
	{
		camPos.x -= speed * deltaTime;
	}
	if (InputManager::KeyDown(InputManager::Right))
	{
		camPos.x += speed * deltaTime;
	}
}

void Game::Draw()
{
	ClearBackground(Color{17, 14, 12, 255});
	
	DrawLevel();
	
}

void Game::DrawLevel()
{
	const int cellSize = 100;

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			Color color = Color();
			
			switch (map[x + y * size.x])
			{
			case MapMaker::Tile::path:
				color = Color{68, 67, 65, 255};
				break;
			case MapMaker::Tile::wall:
				color = Color{42, 40, 39, 255};
				break;
			case MapMaker::Tile::exit:
				color = Color{ 80, 57, 55, 255 };
				break;
			case MapMaker::Tile::vault:
				color = BLUE;
				break;
			case MapMaker::Tile::cell:
				color = YELLOW;
				break;
			case MapMaker::Tile::lever:
				color = GREEN;
				break;
			case MapMaker::Tile::key:
				color = ORANGE;
				break;
			}
			DrawRectangle(cellSize * x - camPos.x, cellSize * y - camPos.y, cellSize, cellSize, color);
		}
	}

	//
}