#include "Game.h"

#include "AABB.h"

#include "InputManager.h"


const float Game::castTime = 0.5f;

Game::Game()
{
	m_map.CreateMap(std::time(nullptr));
	m_map.SetCellSize(100);

	m_minimap.LoadMap(m_map);

	m_player.position = v2(m_map.PlayerSpawn().x, m_map.PlayerSpawn().y) * m_map.CellSize();
	m_camPos = m_player.position - v2(WINDOWX / 2, WINDOWY / 2);

	m_background = LoadTexture("Assets\\Images\\Background.png");

	m_fireballcast = LoadSound("Assets\\Sound\\FireBallCast.mp3");

	InputManager::EnableBind(InputManager::Up);
	InputManager::EnableBind(InputManager::Down);
	InputManager::EnableBind(InputManager::Left);
	InputManager::EnableBind(InputManager::Right);
	InputManager::EnableBind(InputManager::Cast);
}

Game::~Game()
{
	UnloadTexture(m_background);
	UnloadSound(m_fireballcast);
}

void Game::Update(float deltaTime)
{
	m_player.Update(deltaTime);
	
	m_minimap.DiscoverTile(m_map.Vector2ToNode(m_player.position));



	//FireBalls
	if (m_casting)
	{
		m_castingTimer += deltaTime;
		v2 direction = v2();

		direction.x = GetMousePosition().x - WINDOWX / 2;
		direction.y = GetMousePosition().y - WINDOWY / 2;
		direction = direction.Normalized();
		m_fireballs.back().UpdateCastingInfo(m_player.position, direction, m_castingTimer / castTime);

		if (m_castingTimer >= castTime)
			ShootFireBall();
	}

	for (auto iter = m_fireballs.begin(); iter != m_fireballs.end(); iter++)
	{
		iter->Update(deltaTime);
	}

	if (InputManager::KeyPressed(InputManager::Cast) && !m_casting) CastFireBall();


	PhysicStep();
	m_camPos = m_player.position - v2(WINDOWX / 2, WINDOWY / 2);
}

void Game::Draw()
{	
	int offsetX = - int(round(m_camPos.x)) % m_background.width;
	int offsetY = - int(round(m_camPos.y)) % m_background.height;
	
	if (offsetX > 0) offsetX -= m_background.width;
	if (offsetY > 0) offsetY -= m_background.height;

	for (int i = 0; i < 9; i++)
	{
		int x = i % 3 - 1;
		int y = i / 3 - 1;
    
		x = (WINDOWX / 2) + x * m_background.width + offsetX;
		y = (WINDOWY / 2) + y * m_background.height + offsetY;

		DrawTexture(m_background, x, y, WHITE);
	}


	m_map.DrawTiles(m_camPos);
	m_player.Draw(m_camPos);

	for (auto iter = m_fireballs.begin(); iter != m_fireballs.end(); iter++)
	{
		iter->Draw(m_camPos);
	}


	DrawUI();

#ifdef _DEBUG
	DrawFPS(20, 20);
#endif
}

void Game::DrawUI()
{
	m_minimap.Draw(m_map.Vector2ToNode(m_player.position));
}

void Game::PhysicStep()
{
	//CheckMapCollisions
	CheckMapCollisions(&m_player, true);
	
	auto iter = m_fireballs.begin();
	while (iter != m_fireballs.end())
	{
		if(iter->Casting() || !CheckMapCollisions(&*iter, false))
		{
			iter++;
			continue;
		}

		if (&m_fireballs.back() == &*iter)
			StopSound(m_fireballcast);
		iter = m_fireballs.erase(iter);
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

		direction = direction + m_map.Vector2ToNode(obj->Hitbox()->position);
		int index = direction.x + direction.y * m_map.Size().x;
		if (index < 0 || index >= m_map.Size().x * m_map.Size().y) continue;
		if (m_map[index] != Map::Tile::wall) continue;

		AABB tileAABB = AABB();
		tileAABB.position = v2(direction.x * m_map.CellSize() + m_map.CellSize() / 2, direction.y * m_map.CellSize() + m_map.CellSize() / 2);
		tileAABB.size = v2(m_map.CellSize());

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


void Game::CastFireBall()
{
	m_casting = true;
	m_castingTimer = 0;
	PlaySound(m_fireballcast);

	v2 direction = v2();
	direction.x = GetMousePosition().x - WINDOWX / 2;
	direction.y = GetMousePosition().y - WINDOWY / 2;
	direction = direction.Normalized();

	m_fireballs.push_back(FireBall(true, m_player.position, direction));
}

void Game::ShootFireBall()
{
	m_casting = false;
	m_fireballs.back().Fire();
}