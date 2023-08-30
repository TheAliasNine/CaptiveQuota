#include "Captive.h"

#include "AABB.h"

const float Captive::scale = 0.5f;


Captive::Captive()
{
	m_alive = false;

	m_txtrAlive = LoadTexture("Assets\\Images\\Captive.png");
	m_txtrDead = LoadTexture("Assets\\Images\\CaptiveCorpse.png");
}
Captive::Captive(v2 position)
{
	this->position = position;
	m_alive = true;

	m_txtrAlive = LoadTexture("Assets\\Images\\Captive.png");
	m_txtrDead = LoadTexture("Assets\\Images\\CaptiveCorpse.png");

	AABB* hitbox = new AABB();
	hitbox->position = position;
	hitbox->size = v2(m_txtrAlive.width * scale, m_txtrAlive.height * scale);

	m_hitbox = hitbox;
}
#pragma region Rule of 5
Captive::~Captive()
{
	UnloadTexture(m_txtrAlive);
	UnloadTexture(m_txtrDead);
	delete m_hitbox;
}
Captive::Captive(const Captive& other)
{
	position = other.position;
	m_alive = other.m_alive;

	m_txtrAlive = LoadTexture("Assets\\Images\\Captive.png");
	m_txtrDead = LoadTexture("Assets\\Images\\CaptiveCorpse.png");

	m_hitbox = new AABB(*static_cast<AABB*>(other.m_hitbox));
}

Captive& Captive::operator= (const Captive& other)
{
	position = other.position;
	m_alive = other.m_alive;

	m_hitbox = new AABB(*static_cast<AABB*>(other.m_hitbox));

	return *this;
}
Captive::Captive(Captive&& other)
{
	position = other.position;
	m_alive = other.m_alive;

	m_txtrAlive = LoadTexture("Assets\\Images\\Captive.png");
	m_txtrDead = LoadTexture("Assets\\Images\\CaptiveCorpse.png");

	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
}
Captive& Captive::operator= (Captive&& other)
{
	position = other.position;
	m_alive = other.m_alive;

	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;

	return *this;
}
#pragma endregion

void Captive::Update(float deltaTime)
{
	if (!m_alive) return;
}
void Captive::Draw(v2 camPos)
{
	Vector2 drawPos = Vector2{ position.x - camPos.x, position.y - camPos.y };
	if (m_alive)
	{
		drawPos.x -= m_txtrAlive.width / 2 * scale;
		drawPos.y -= m_txtrAlive.height / 2 * scale;
		DrawTextureEx(m_txtrAlive, drawPos, 0, scale, WHITE);
	}
	else
	{
		drawPos.x -= m_txtrDead.width / 2 * scale;
		drawPos.y -= m_txtrDead.height / 2 * scale;

		drawPos.y += m_txtrAlive.height / 2 - m_txtrDead.height;
		DrawTextureEx(m_txtrDead, drawPos, 0, scale, WHITE);
	}
}

void Captive::Kill()
{
	m_alive = false;
}