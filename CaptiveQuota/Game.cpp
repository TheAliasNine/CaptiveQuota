#include "Game.h"

#include "AABB.h"

#include "InputManager.h"


Game::Game()
{
	map.CreateMap(std::time(nullptr));
	map.SetCellSize(100);

	minimap = Minimap();
	minimap.LoadMap(map);

	player.position = v2(map.PlayerSpawn().x, map.PlayerSpawn().y) * map.CellSize();
	camPos = player.position - v2(WINDOWX / 2, WINDOWY / 2);

	background = LoadTexture("Assets\\Images\\Background.png");

	InputManager::EnableBind(InputManager::Up);
	InputManager::EnableBind(InputManager::Down);
	InputManager::EnableBind(InputManager::Left);
	InputManager::EnableBind(InputManager::Right);
	InputManager::EnableBind(InputManager::Cast);
}

Game::~Game()
{
	UnloadTexture(background);
}

void Game::Update(float deltaTime)
{
	player.Update(deltaTime);
	
	minimap.DiscoverTile(map.Vector2ToNode(player.position));

	//FireBalls
	for (auto iter = fireballs.begin(); iter != fireballs.end(); iter++)
	{
		iter->Update(deltaTime);
	}

	if (InputManager::KeyPressed(InputManager::Cast))
	{
		ShootFireBall();
	}

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
	player.Draw(camPos);

	for (auto iter = fireballs.begin(); iter != fireballs.end(); iter++)
	{
		iter->Draw(camPos);
	}


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
	//CheckMapCollisions
	CheckMapCollisions(&player, true);
	
	auto iter = fireballs.begin();
	while (iter != fireballs.end())
	{
		if(!CheckMapCollisions(&*iter, false))
		{
			iter++;
			continue;
		}

		iter = fireballs.erase(iter);
	}
}

//Assumes that the hitbox is no larger than the size of 2xCellSize
bool Game::CheckMapCollisions(HitBoxObject* obj, bool resolve)
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

		direction = direction + map.Vector2ToNode(obj->Hitbox()->position);
		int index = direction.x + direction.y * map.Size().x;
		if (index < 0 || index >= map.Size().x * map.Size().y) continue;
		if (map[index] != Map::Tile::wall) continue;

		AABB tileAABB = AABB();
		tileAABB.position = v2(direction.x * map.CellSize() + map.CellSize() / 2, direction.y * map.CellSize() + map.CellSize() / 2);
		tileAABB.size = v2(map.CellSize());

		if (!resolve)
		{
			if (obj->Hitbox()->CheckCollision(&tileAABB, nullptr)) return true;
			continue;
		}

		CollisionInfo info = CollisionInfo();
		Collider* hitbox = obj->Hitbox();
		hitbox->position = hitbox->position + hitboxOffset;
		bool collided = hitbox->CheckCollision(&tileAABB, &info);
		hitbox->position = hitbox->position - hitboxOffset;
		if (!collided) continue;
		hitboxOffset = hitboxOffset + info.depth * info.direction;
	}
	obj->position = obj->position + hitboxOffset;
	return hitboxOffset.x != 0 || hitboxOffset.y != 0;
}


void Game::ShootFireBall()
{
	v2 direction = v2();
	direction.x = GetMousePosition().x - WINDOWX / 2;
	direction.y = GetMousePosition().y - WINDOWY / 2;
	direction = direction.Normalized();

	fireballs.push_back(FireBall(player.position, direction));
}