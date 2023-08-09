#include "Game.h"

#include "InputManager.h"

#include "raylib.h"

Game::Game()
{
	map = Map();
	map.CreateMap(std::time(nullptr));

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
	const int cellSize = 5;

	for (int y = 0; y < map.Size().y; y++)
	{
		for (int x = 0; x < map.Size().x; x++)
		{
			Color color = Color();
			
			switch (map[x + y * map.Size().x])
			{
			case Map::Tile::path:
				color = Color{68, 67, 65, 255};
				break;
			case Map::Tile::wall:
				color = Color{42, 40, 39, 255};
				break;
			case Map::Tile::exit:
				color = RED;
				//color = Color{ 80, 57, 55, 255 };
				break;
			case Map::Tile::vault:
				color = BLUE;
				break;
			case Map::Tile::cell:
				color = YELLOW;
				break;
			case Map::Tile::lever:
				color = GREEN;
				break;
			case Map::Tile::key:
				color = ORANGE;
				break;
			}
			DrawRectangle(cellSize * x - camPos.x, cellSize * y - camPos.y, cellSize, cellSize, color);
		}
	}

	//
}