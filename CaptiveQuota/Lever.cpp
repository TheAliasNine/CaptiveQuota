#include "Lever.h"

#include "AABB.h"

const float Lever::scale = 0.3f;
const float Lever::speed = 8;
const float Lever::border = 0.5f;

Lever::Lever()
{
	position = v2();
	m_map = nullptr;

	m_textures[0] = LoadTexture("Assets\\Images\\SwitchBody.png");
	m_textures[1] = LoadTexture("Assets\\Images\\SwitchHandle.png");

	m_leverOn = LoadSound("Assets\\Sound\\LeverOn.wav");
	m_leverOff = LoadSound("Assets\\Sound\\LeverOff.wav");
}

Lever::Lever(Map* map, int leverInd)
{
	m_map = map;
	m_leverPos = m_map->LeverPos(leverInd);

	position = m_map->NodeToVector2(m_leverPos);

	m_handlePos = m_map->IsLeverActive(m_leverPos) ? -1 : 1;

	m_textures[0] = LoadTexture("Assets\\Images\\SwitchBody.png");
	m_textures[1] = LoadTexture("Assets\\Images\\SwitchHandle.png");

	m_leverOn = LoadSound("Assets\\Sound\\LeverOn.wav");
	SetSoundVolume(m_leverOn, 0.7f);
	m_leverOff = LoadSound("Assets\\Sound\\LeverOff.wav");
	SetSoundVolume(m_leverOff, 0.7f);
	
	AABB* hitbox = new AABB();
	hitbox->position = position;
	hitbox->size = v2(m_textures[0].width * scale, m_textures[0].height * scale);
	m_hitbox = hitbox;
}

#pragma region Rule of 5
Lever::~Lever()
{
	for (int i = 0; i < textureCount; i++)
	{
		UnloadTexture(m_textures[0]);
	}
	UnloadSound(m_leverOn);
	UnloadSound(m_leverOff);
}

Lever::Lever(const Lever& other)
{
	m_map = other.m_map;
	m_leverPos = other.m_leverPos;
	m_handlePos = other.m_handlePos;
	position = other.position;

	m_textures[0] = LoadTexture("Assets\\Images\\SwitchBody.png");
	m_textures[1] = LoadTexture("Assets\\Images\\SwitchHandle.png");
	m_leverOn = LoadSound("Assets\\Sound\\LeverOn.wav");
	SetSoundVolume(m_leverOn, 0.7f);
	m_leverOff = LoadSound("Assets\\Sound\\LeverOff.wav");
	SetSoundVolume(m_leverOff, 0.7f);

	m_hitbox = new AABB(*static_cast<AABB*>(m_hitbox));
}

Lever& Lever::operator= (const Lever& other)
{
	m_map = other.m_map;
	m_leverPos = other.m_leverPos;
	m_handlePos = other.m_handlePos;
	position = other.position;

	delete m_hitbox;
	m_hitbox = new AABB(*static_cast<AABB*>(m_hitbox));

	return *this;
}

Lever::Lever(Lever&& other)
{
	m_map = other.m_map;
	m_leverPos = other.m_leverPos;
	m_handlePos = other.m_handlePos;
	position = other.position;

	m_textures[0] = LoadTexture("Assets\\Images\\SwitchBody.png");
	m_textures[1] = LoadTexture("Assets\\Images\\SwitchHandle.png");

	m_leverOn = LoadSound("Assets\\Sound\\LeverOn.wav");
	SetSoundVolume(m_leverOn, 0.7f);
	m_leverOff = LoadSound("Assets\\Sound\\LeverOff.wav");
	SetSoundVolume(m_leverOff, 0.7f);

	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
}

Lever& Lever::operator= (Lever&& other)
{
	m_map = other.m_map;
	m_leverPos = other.m_leverPos;
	m_handlePos = other.m_handlePos;
	position = other.position;
	delete m_hitbox;
	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
	return *this;
}
#pragma endregion

void Lever::Update(float deltaTime)
{
	float goal = m_map->IsLeverActive(m_leverPos) ? -1 : 1;
	if (m_handlePos == goal) return;
	m_handlePos += speed * goal * deltaTime;
	if (m_handlePos > 1) m_handlePos = goal;
	if (m_handlePos < -1) m_handlePos = goal;
}

void Lever::Draw(v2 camPos)
{
	//Draw Base
	Vector2 basePos = Vector2{ position.x - camPos.x - (m_textures[0].width / 2 * scale), position.y - camPos.y - (m_textures[0].height / 2 * scale) };
	DrawTextureEx(m_textures[0], basePos, 0, scale, WHITE);

	//Draw Handle
	Vector2 handlePos = Vector2{ position.x - camPos.x - (m_textures[1].width / 2 * scale), position.y - camPos.y - (m_textures[1].height / 2 * scale) };
	handlePos.y += m_handlePos * (m_textures[0].height / 2 * scale * (1-border));
	DrawTextureEx(m_textures[1], handlePos, 0, scale, WHITE);
}

void Lever::Interact()
{
	if (m_map->IsLeverActive(m_leverPos))
		TurnOff();
	else
		TurnOn();
}

void Lever::TurnOff()
{
	if (!m_map->IsLeverActive(m_leverPos)) return;
	PlaySound(m_leverOff);
	m_map->DeactivateLever(m_leverPos);
}

void Lever::TurnOn()
{
	if (m_map->IsLeverActive(m_leverPos)) return;
	PlaySound(m_leverOn);
	m_map->ActivateLever(m_leverPos);
}

//TODO: then do theme music and key mold