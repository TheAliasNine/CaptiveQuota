#include "Explosion.h"

#include "Circle.h"

const float Explosion::liveTime = 1;
const float Explosion::scale = 4;

Explosion::Explosion()
{
	m_alive = false;
	m_timer = 0;
	for (int i = 0; i < frameCount; i++)
	{
		m_animFrames[i] = LoadTexture(FormatText("Assets\\Images\\Exploaion%i", i));
	}
}
#pragma region Rule of 5
Explosion::~Explosion()
{
	for (int i = 0; i < frameCount; i++)
	{
		UnloadTexture(m_animFrames[i]);
	}
}
Explosion::Explosion(const Explosion& other)
{
	m_alive = other.m_alive;
	m_timer = other.m_timer;
	for (int i = 0; i < frameCount; i++)
	{
		m_animFrames[i] = LoadTexture(FormatText("Assets\\Images\\Explosion%i", i));
	}
	if (other.m_hitbox != nullptr)
		m_hitbox = new Circle(*static_cast<Circle*>(other.m_hitbox));
}
Explosion& Explosion::operator= (const Explosion& other)
{
	m_alive = other.m_alive;
	m_timer = other.m_timer;
	delete m_hitbox;
	if (other.m_hitbox != nullptr)
		m_hitbox = new Circle(*static_cast<Circle*>(other.m_hitbox));

	return *this;
}
Explosion::Explosion(const Explosion&& other)
{
	m_alive = other.m_alive;
	m_timer = other.m_timer;
	for (int i = 0; i < frameCount; i++)
	{
		m_animFrames[i] = LoadTexture(FormatText("Assets\\Images\\Explosion%i.png", i));
	}
	if (other.m_hitbox != nullptr)
		m_hitbox = new Circle(*static_cast<Circle*>(other.m_hitbox));

}
Explosion& Explosion::operator= (const Explosion&& other)
{
	m_alive = other.m_alive;
	m_timer = other.m_timer;

	delete m_hitbox;
	if (other.m_hitbox != nullptr)
		m_hitbox = new Circle(*static_cast<Circle*>(other.m_hitbox));
	return *this;
}
#pragma endregion


void Explosion::Init(v2 position)
{
	m_alive = true;
	this->position = position;

	m_hitbox = new Circle();
	m_hitbox->position = position;
}

void Explosion::Update(float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer >= liveTime)
		m_alive = false;

	Circle* hitbox = static_cast<Circle*>(m_hitbox);
	hitbox->position = this->position;
	int frame = m_timer / (liveTime / frameCount);
	if (frame >= frameCount) frame = frameCount - 1;
	if (frame < 0) frame = 0;
	hitbox->radius = m_animFrames[frame].width / 2 * scale;
}

void Explosion::Draw(v2 camPos)
{
	int frame = m_timer / (liveTime / frameCount);
	if (frame >= frameCount) frame = frameCount - 1;
	if (frame < 0) frame = 0;
	Vector2 drawPos = Vector2{ position.x - camPos.x - m_animFrames[frame].width / 2 * scale, position.y - camPos.y - m_animFrames[frame].height / 2 * scale };
	DrawTextureEx(m_animFrames[frame], drawPos, 0, scale, WHITE);
}