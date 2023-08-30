#include "KeyMaker.h"

#include "AABB.h"

const float KeyMaker::scale = 1;

KeyMaker::KeyMaker()
{
	position = v2();
	m_map = nullptr;

	m_textures[0] = LoadTexture("Assets\\Images\\KeyMaker.png");

	m_interacted = LoadSound("Assets\\Sound\\KeyMaker.wav");
}

KeyMaker::KeyMaker(Map* map, int leverInd)
{
	m_map = map;
	m_keyMakerPos = m_map->KeyMakerPos(leverInd);

	position = m_map->NodeToVector2(m_keyMakerPos);


	m_textures[0] = LoadTexture("Assets\\Images\\KeyMaker.png");

	m_interacted = LoadSound("Assets\\Sound\\KeyMaker.wav");

	AABB* hitbox = new AABB();
	hitbox->position = position;
	hitbox->size = v2(m_textures[0].width * scale, m_textures[0].height * scale);
	m_hitbox = hitbox;
}

#pragma region Rule of 5
KeyMaker::~KeyMaker()
{
	for (int i = 0; i < textureCount; i++)
	{
		UnloadTexture(m_textures[0]);
	}
	UnloadSound(m_interacted);
}

KeyMaker::KeyMaker(const KeyMaker& other)
{
	m_map = other.m_map;
	m_keyMakerPos = other.m_keyMakerPos;
	position = other.position;

	m_textures[0] = LoadTexture("Assets\\Images\\KeyMaker.png");

	m_interacted = LoadSound("Assets\\Sound\\KeyMaker.wav");


	m_hitbox = new AABB(*static_cast<AABB*>(m_hitbox));
}

KeyMaker& KeyMaker::operator= (const KeyMaker& other)
{
	m_map = other.m_map;
	m_keyMakerPos = other.m_keyMakerPos;
	position = other.position;

	delete m_hitbox;
	m_hitbox = new AABB(*static_cast<AABB*>(m_hitbox));

	return *this;
}

KeyMaker::KeyMaker(KeyMaker&& other)
{
	m_map = other.m_map;
	m_keyMakerPos = other.m_keyMakerPos;
	position = other.position;

	m_textures[0] = LoadTexture("Assets\\Images\\KeyMaker.png");

	m_interacted = LoadSound("Assets\\Sound\\KeyMaker.wav");

	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
}

KeyMaker& KeyMaker::operator= (KeyMaker&& other)
{
	m_map = other.m_map;
	m_keyMakerPos = other.m_keyMakerPos;
	position = other.position;
	delete m_hitbox;
	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
	return *this;
}
#pragma endregion

void KeyMaker::Draw(v2 camPos)
{
	//Draw Base
	Vector2 drawPos = Vector2{ position.x - camPos.x - (m_textures[0].width / 2 * scale), position.y - camPos.y - (m_textures[0].height / 2 * scale) };
	DrawTextureEx(m_textures[0], drawPos, 0, scale, WHITE);
}

void KeyMaker::Interact()
{
	PlaySound(m_interacted);
}