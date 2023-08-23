#include "Game.h"

#include "InputManager.h"


Game::Game()
{
	map = Map();
	map.CreateMap(std::time(nullptr));
	map.SetCellSize(100);

	minimap = Minimap();
	minimap.LoadMap(map);

	camPos = v2(map.PlayerSpawn().x * map.CellSize(), map.PlayerSpawn().y * map.CellSize());
	player = Player();
	player.position = v2(map.PlayerSpawn().x, map.PlayerSpawn().y) * map.CellSize();

	background = LoadTexture("Assets\\Images\\Background.png");

	InputManager::EnableBind(InputManager::Up);
	InputManager::EnableBind(InputManager::Down);
	InputManager::EnableBind(InputManager::Left);
	InputManager::EnableBind(InputManager::Right);
}

Game::~Game()
{
	UnloadTexture(background);
}

void Game::Update(float deltaTime)
{
	player.Update(deltaTime);
	
	minimap.DiscoverTile(map.Vector2ToNode(player.position));

	PhysicStep();
	camPos = player.position - v2(WINDOWX / 2, WINDOWY / 2);
}

void Game::Draw()
{	
	int offsetX = - int(round(camPos.x)) % background.width;
	int offsetY = - int(round(camPos.y)) % background.height;
	
	if (offsetX > 0) offsetX -= background.width;
	if (offsetY > 0) offsetY -= background.height;

	for (int i = 0; i < 9; i++)
	{
		int x = i % 3 - 1;
		int y = i / 3 - 1;
    
		x = (WINDOWX / 2) + x * background.width + offsetX;
		y = (WINDOWY / 2) + y * background.height + offsetY;

		DrawTexture(background, x, y, WHITE);
	}


	map.DrawTiles(camPos);
	player.Draw();

	DrawUI();

#ifdef _DEBUG
	DrawFPS(20, 20);
#endif
}

void Game::DrawUI()
{
	minimap.Draw(map.Vector2ToNode(player.position));
}

void Game::PhysicStep()
{
	//move player to not collide with any tiles
	ResolveMapCollisions(&player);
	//do the same for the NPCs
}

#include <stdio.h>

//Assumes that the hitbox is no larger than the size of 2xCellSize
void Game::ResolveMapCollisions(BoxObject* obj)
{
	v2 hitboxOffset = v2();
	for (int i = 0; i < 9; i++)
	{
		intV2 direction;
		switch (i)
		{
		case(0):
			direction = intV2{ 0, 0 };
			break;
		case(1):
			direction = intV2{ 1, 0 };
			break;
		case(2):
			direction = intV2{ -1, 0 };
			break;
		case(3):
			direction = intV2{ 0, 1 };
			break;
		case(4):
			direction = intV2{ 0, -1 };
			break;
		case(5):
			direction = intV2{ -1, 1 };
			break;
		case(6):
			direction = intV2{ 1, 1 };
			break;
		case(7):
			direction = intV2{ 1, -1 };
			break;
		case(8):
			direction = intV2{ -1, -1 };
			break;
		}

		direction = direction + map.Vector2ToNode(obj->Hitbox().position);
		int index = direction.x + direction.y * map.Size().x;
		if (map[index] != Map::Tile::wall) continue;

		AABB tileAABB = AABB();
		tileAABB.position = v2(direction.x * map.CellSize() + map.CellSize() / 2, direction.y * map.CellSize() + map.CellSize() / 2);
		tileAABB.size = v2(map.CellSize());

		CollisionInfo info = CollisionInfo();
		AABB hitbox = obj->Hitbox();
		hitbox.position = hitbox.position + hitboxOffset;
		if (!hitbox.CheckCollision(&tileAABB, &info)) continue;
		hitboxOffset = hitboxOffset + info.depth * info.direction;
	}
	obj->position = obj->position + hitboxOffset;
}