#include "Game.h"

#include "AABB.h"
#include "MainMenu.h"

#include "InputManager.h"


const float Game::castTime = 0.5f;
const float Game::portalAnimTime = 0.7f;
const float Game::portalScale = 4;

Game::Game()
{
	music = LoadMusicStream("Assets\\Sound\\MainMusic.ogg");
	PlayMusicStream(music);

	m_map.CreateMap(std::time(nullptr));
	m_map.SetCellSize(100);

	m_minimap.LoadMap(m_map);

	m_player.position = v2(m_map.PlayerSpawn().x, m_map.PlayerSpawn().y) * m_map.CellSize();
	m_camPos = m_player.position - v2(WINDOWX / 2, WINDOWY / 2);

	m_exitObj = Exit(&m_map);

	m_background = LoadTexture("Assets\\Images\\Background.png");

	m_fireballcast = LoadSound("Assets\\Sound\\FireBallCast.mp3");
	m_explosion = LoadSound("Assets\\Sound\\Explosion.wav");

	for (int i = 0; i < portalFrames; i++)
	{
		m_portalFrame[i] = LoadTexture(TextFormat("Assets\\Images\\Portal%i.png", i));
	}

	SetMasterVolume(0.5f);

	InputManager::EnableBind(InputManager::Up);
	InputManager::EnableBind(InputManager::Down);
	InputManager::EnableBind(InputManager::Left);
	InputManager::EnableBind(InputManager::Right);
	InputManager::EnableBind(InputManager::Cast);
	InputManager::EnableBind(InputManager::Interact);
	InputManager::EnableBind(InputManager::FreeCam);

	m_levers.reserve(m_map.LeverCount());
	for (int i = 0; i < m_map.LeverCount(); i++)
	{
		m_levers.push_back(Lever(&m_map, i));
	}
	m_keyMakers.reserve(m_map.KeyMakerCount());
	for (int i = 0; i < m_map.KeyMakerCount(); i++)
	{
		m_keyMakers.push_back(KeyMaker(&m_map, i));
	}
}

Game::~Game()
{
	UnloadTexture(m_background);
	UnloadSound(m_fireballcast);
	UnloadSound(m_explosion);

	for (int i = 0; i < portalFrames; i++)
	{
		UnloadTexture(m_portalFrame[i]);
	}

	StopMusicStream(music);
	UnloadMusicStream(music);
}

void Game::Update(float deltaTime)
{
	UpdateMusicStream(music);

	if (m_gameFinished)
	{
		if (InputManager::KeyDown(InputManager::Restart))
			Restart();
		if (InputManager::KeyDown(InputManager::Exit))
			Close();

		return;
	}

	if (InputManager::KeyPressed(InputManager::FreeCam))
		m_freeCam = !m_freeCam;

	if (!m_freeCam)
	{
		m_player.Update(deltaTime);
		m_camPos = m_player.position - v2(WINDOWX / 2, WINDOWY / 2);
	}
	else
	{
		v2 moveDir = v2();
		if (InputManager::KeyDown(InputManager::Up))
			moveDir.y -= 1;
		if (InputManager::KeyDown(InputManager::Down))
			moveDir.y += 1;
		if (InputManager::KeyDown(InputManager::Left))
			moveDir.x -= 1;
		if (InputManager::KeyDown(InputManager::Right))
			moveDir.x += 1;

		moveDir = moveDir.Normalized();
		m_camPos = m_camPos + moveDir * 1000 * deltaTime;
	}
	
	m_map.Update(deltaTime);

	//Use hitbox position to avoid getting cells not on due to topdown perspective
	m_minimap.DiscoverTile(m_map.Vector2ToNode(m_player.Hitbox()->position));
	
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
	auto explIter = m_explosions.begin();
	while (explIter != m_explosions.end())
	{
		if (explIter->m_alive)
		{
			explIter->Update(deltaTime);
			explIter++;
			continue;
		}
		explIter = m_explosions.erase(explIter);
	}
	if (InputManager::KeyPressed(InputManager::Cast) && !m_casting) CastFireBall();

	m_exitObj.Update(deltaTime);

	for (auto iter = m_levers.begin(); iter != m_levers.end(); iter++)
	{
		iter->Update(deltaTime);
	}

	if (InputManager::KeyDown(InputManager::Interact))
	{
		for (auto iter = m_levers.begin(); iter != m_levers.end(); iter++)
		{
			if (!iter->Hitbox()->CheckCollision(m_camPos + v2(GetMouseX(), GetMouseY()))) continue;
			iter->TurnOff();
		}
	}

	PhysicStep();
}

void Game::Draw(float deltaTime)
{	
	DrawBackground();

	m_map.DrawTiles(m_camPos);

	m_exitObj.Draw(m_camPos);

	for (auto iter = m_levers.begin(); iter != m_levers.end(); iter++)
	{
		iter->Draw(m_camPos);
	}
	for (auto iter = m_keyMakers.begin(); iter != m_keyMakers.end(); iter++)
	{
		iter->Draw(m_camPos);
	}

	if (m_map.IsPortalActive())
	{
		m_portalAnimTimer += deltaTime;
		if (m_portalAnimTimer > portalAnimTime) m_portalAnimTimer -= portalAnimTime;
		int frame = int(m_portalAnimTimer / (portalAnimTime / portalFrames));
		v2 halfFrameDimensions = v2(m_portalFrame[frame].width / 2 * portalScale, m_portalFrame[frame].height / 2 * portalScale);
		v2 drawPos = m_map.NodeToVector2(m_map.PortalPosition()) - m_camPos - halfFrameDimensions;

		if ((drawPos.x + halfFrameDimensions.x > 0 || drawPos.x - halfFrameDimensions.x < WINDOWX) && (drawPos.y + halfFrameDimensions.y > 0 || drawPos.y - halfFrameDimensions.y < WINDOWY))
		{
			Vector2 position = Vector2{ drawPos.x, drawPos.y };

			DrawTextureEx(m_portalFrame[frame], position, 0, portalScale, WHITE);
		}
	}

	

	m_player.Draw(m_camPos);

	for (auto iter = m_fireballs.begin(); iter != m_fireballs.end(); iter++)
	{
		iter->Draw(m_camPos);
	}
	for (auto iter = m_explosions.begin(); iter != m_explosions.end(); iter++)
	{
		iter->Draw(m_camPos);
	}


	DrawUI();


	if (m_gameFinished)
	{
		DrawRectangle(0, 0, WINDOWX, WINDOWY, Color{ 0,0,0,128 });

		if (m_successful)
		{
			int width = MeasureText("You obtained enough sacrifices.", 60);
			DrawText("You obtained enough sacrifices.", (WINDOWX - width) / 2, 50, 60, Color{ 90, 40, 40, 255 });
		}
		else
		{
			int width = MeasureText("Too many captives escaped.", 60);
			DrawText("Too many captives escaped.", (WINDOWX - width) / 2, 50, 60, Color{ 90, 40, 40, 255 });
		}
		DrawText("Press 'r' to restart", 20, 550, 40, Color {90, 40, 40, 255});
		DrawText("Press 'esc' to exit", 20, 600, 40, Color {90, 40, 40, 255});
	}


#ifdef _DEBUG
	DrawFPS(20, 20);
#endif
}

void Game::DrawBackground()
{
	int offsetX = -int(round(m_camPos.x)) % m_background.width;
	int offsetY = -int(round(m_camPos.y)) % m_background.height;

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
		CreateExplosion(iter->position);
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
		if (direction.x < 0 || direction.x >= m_map.Size().x || direction.y < 0 || direction.y >= m_map.Size().y)
			continue;
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
	if (resolve)
	{
		CollisionInfo info = CollisionInfo();
		Collider* hitbox = obj->Hitbox();
		hitbox->position = hitbox->position + hitboxOffset;
		bool collided = hitbox->CheckCollision(m_exitObj.Hitbox(), &info);
		if (collided)
		{
			hitboxOffset = hitboxOffset + info.depth * info.direction;
		}
	}
	else
	{
		Collider* hitbox = obj->Hitbox();
		hitbox->position = hitbox->position + hitboxOffset;
		if (hitbox->CheckCollision(m_exitObj.Hitbox(), nullptr)) return true;
	}
	//levers
	for (auto iter = m_levers.begin(); iter != m_levers.end(); iter++)
	{
		if (resolve)
		{
			CollisionInfo info = CollisionInfo();
			Collider* hitbox = obj->Hitbox();
			hitbox->position = hitbox->position + hitboxOffset;
			bool collided = hitbox->CheckCollision(iter->Hitbox(), &info);
			if (collided)
			{
				hitboxOffset = hitboxOffset + info.depth * info.direction;
			}
		}
		else
		{
			Collider* hitbox = obj->Hitbox();
			hitbox->position = hitbox->position + hitboxOffset;
			if (hitbox->CheckCollision(iter->Hitbox(), nullptr)) return true;
		}
	}

	//KeyMakers
	for (auto iter = m_keyMakers.begin(); iter != m_keyMakers.end(); iter++)
	{
		if (resolve)
		{
			CollisionInfo info = CollisionInfo();
			Collider* hitbox = obj->Hitbox();
			hitbox->position = hitbox->position + hitboxOffset;
			bool collided = hitbox->CheckCollision(iter->Hitbox(), &info);
			if (collided)
			{
				hitboxOffset = hitboxOffset + info.depth * info.direction;
			}
		}
		else
		{
			Collider* hitbox = obj->Hitbox();
			hitbox->position = hitbox->position + hitboxOffset;
			if (hitbox->CheckCollision(iter->Hitbox(), nullptr)) return true;
		}
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

void Game::CreateExplosion(v2 position)
{
	PlaySound(m_explosion);
	m_explosions.push_back(Explosion());
	m_explosions.back().Init(position);
}

void Game::GameFinished(bool successful)
{
	InputManager::EnableBind(InputManager::Restart);
	InputManager::EnableBind(InputManager::Exit);
	m_gameFinished = true;
	m_successful = successful;

	
}

void Game::Restart()
{
	nextState = new Game();
	closed = true;
}
void Game::Close()
{
	nextState = new MainMenu();
	closed = true;
}

void Game::Interact()
{
	for (auto iter = m_levers.begin(); iter != m_levers.end(); iter++)
	{
		iter->Draw(m_camPos);
	}
}