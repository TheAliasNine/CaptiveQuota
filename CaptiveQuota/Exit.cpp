#include "Exit.h"

#include "AABB.h"

const float Exit::scale = 1;

Exit::Exit()
{
	m_map = nullptr;

	m_textures[0] = LoadTexture("Assets\\Images\\ExitClosed.png");
	m_textures[1] = LoadTexture("Assets\\Images\\ExitOpen.png");
}

#pragma region Rule of 5
Exit::~Exit()
{
	for (int i = 0; i < textureCount; i++)
	{
		UnloadTexture(m_textures[i]);
	}
}

Exit::Exit(Map* map)
{
	m_map = map;
	m_textures[0] = LoadTexture("Assets\\Images\\ExitClosed.png");
	m_textures[1] = LoadTexture("Assets\\Images\\ExitOpen.png");

	position = m_map->NodeToVector2(m_map->Exit());

	AABB* hitbox = new AABB();
	hitbox->size = v2(m_textures[0].width * scale, m_textures[0].height * scale);
	hitbox->position = position;
	m_hitbox = hitbox;
}

Exit::Exit(const Exit& other)
{
	m_map = other.m_map;
	m_textures[0] = LoadTexture("Assets\\Images\\ExitClosed.png");
	m_textures[1] = LoadTexture("Assets\\Images\\ExitOpen.png");
	position = other.position;
	m_hitbox = new AABB(*static_cast<AABB*>(other.m_hitbox));
}

Exit& Exit::operator= (const Exit& other)
{
	m_map = other.m_map;

	position = other.position;
	delete m_hitbox;
	m_hitbox = new AABB(*static_cast<AABB*>(other.m_hitbox));
	return *this;
}

Exit::Exit(Exit&& other)
{
	m_map = other.m_map;
	m_textures[0] = LoadTexture("Assets\\Images\\ExitClosed.png");
	m_textures[1] = LoadTexture("Assets\\Images\\ExitOpen.png");
	position = other.position;

	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
}

Exit& Exit::operator= (Exit&& other)
{
	m_map = other.m_map;

	position = other.position;
	delete m_hitbox;
	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
	return *this;
}
#pragma endregion


void Exit::Draw(v2 camPos)
{
	int texture = m_map->ExitOpen() ? 1 : 0;
	Vector2 drawPos = Vector2{ position.x - camPos.x - m_textures[0].width / 2, position.y - camPos.y - m_textures[0].height / 2 };
	DrawTextureEx(m_textures[texture], drawPos, 0, scale, WHITE);
}