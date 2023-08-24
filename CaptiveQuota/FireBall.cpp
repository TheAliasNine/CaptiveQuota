#include "FireBall.h"

#include "Circle.h"

#include "raymath.h"

const float FireBall::speed = 1000;
const float FireBall::animationTime = 0.5f;
const float FireBall::scale = 3;


FireBall::FireBall(v2 position, v2 direction)
{
	for (int i = 0; i < frameCount; i++)
	{
		m_animationFrames[i] = LoadTexture(TextFormat("Assets\\Images\\FireBall%i.png", i));
	}
	m_direction = direction;
	this->position = position;

	Circle* hitbox = new Circle();
	hitbox->position = position;
	hitbox->radius = m_animationFrames[0].width / 2 * scale;

	m_hitbox = hitbox;
}

#pragma region Rule of 5
FireBall::~FireBall()
{
	for (int i = 0; i < frameCount; i++)
	{
		UnloadTexture(m_animationFrames[i]);
	}
}

FireBall::FireBall(const FireBall& other)
{
	for (int i = 0; i < frameCount; i++)
	{
		m_animationFrames[i] = LoadTexture(TextFormat("Assets\\Images\\FireBall%i.png", i));
	}
	m_direction = other.m_direction;
	position = other.position;

	Circle* hitbox = new Circle(*static_cast<Circle*>(other.m_hitbox));
	m_hitbox = hitbox;
}

FireBall& FireBall::operator= (const FireBall& other)
{
	m_direction = other.m_direction;
	position = other.position;

	Circle* hitbox = new Circle(*static_cast<Circle*>(other.m_hitbox));
	m_hitbox = hitbox;
	return *this;
}

FireBall::FireBall(FireBall&& other)
{
	for (int i = 0; i < frameCount; i++)
	{
		m_animationFrames[i] = other.m_animationFrames[i];
		other.m_animationFrames[i] = Texture2D();
	}
	m_direction = other.m_direction;
	position = other.position;

	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
}

FireBall& FireBall::operator= (FireBall&& other)
{
	m_direction = other.m_direction;
	position = other.position;

	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
	return *this;
}

#pragma endregion

void FireBall::Update(float deltaTime)
{
	//position = position + speed * m_direction * deltaTime;

	m_timer += deltaTime;

	m_hitbox->position = position;
}

void FireBall::Draw(v2 camPos)
{
	if (m_timer > animationTime) m_timer -= animationTime;

	int frame = (int)(m_timer / (animationTime / frameCount));

	float angle = Vector2Angle(Vector2{ 0, 0 }, Vector2{ m_direction.x, m_direction.y });


	Vector2 halfSize = Vector2{ (float)m_animationFrames[frame].width / 2 * scale, (float)m_animationFrames[frame].width / 2 * scale};
	Vector2 rotated = Vector2Rotate(halfSize, angle);

	Vector2 drawPos = Vector2{ position.x - camPos.x, position.y - camPos.y };
	drawPos = Vector2Subtract(drawPos, rotated);

	DrawTextureEx(m_animationFrames[frame], drawPos, angle, scale, WHITE);
}