#include "FireBall.h"

#include "Circle.h"

#include "raymath.h"

const float FireBall::speed = 1200;
const float FireBall::animationTime = 0.2f;
const float FireBall::scale = 3;
const float FireBall::castOffSet = 50;


FireBall::FireBall(bool casting, v2 position, v2 direction)
{
	m_casting = casting;
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
	m_casting = other.m_casting;
}

FireBall& FireBall::operator= (const FireBall& other)
{
	m_direction = other.m_direction;
	position = other.position;

	Circle* hitbox = new Circle(*static_cast<Circle*>(other.m_hitbox));
	m_hitbox = hitbox;
	m_casting = other.m_casting;

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
	m_casting = other.m_casting;
}

FireBall& FireBall::operator= (FireBall&& other)
{
	m_direction = other.m_direction;
	position = other.position;

	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
	m_casting = other.m_casting;
	return *this;
}

#pragma endregion

void FireBall::Update(float deltaTime)
{
	m_timer += deltaTime;

	if (m_casting)
	{
		
		return;
	}

	position = position + speed * m_direction * deltaTime;
	m_hitbox->position = position;
}

void FireBall::Draw(v2 camPos)
{ 
	if (m_timer > animationTime) m_timer -= animationTime;

	int frame = (int)(m_timer / (animationTime / frameCount));

	float angle = Vector2Angle(Vector2{ 0, 0 }, Vector2{ m_direction.x, m_direction.y });

	if (m_casting)
	{
		Vector2 halfSize = Vector2{ (float)m_animationFrames[frame].width / 2 * scale * m_castingProgress - castOffSet, (float)m_animationFrames[frame].width / 2 * scale * m_castingProgress};
		Vector2 rotated = Vector2Rotate(halfSize, angle);

		Vector2 drawPos = Vector2{ position.x - camPos.x, position.y - camPos.y };
		drawPos = Vector2Subtract(drawPos, rotated);

		DrawTextureEx(m_animationFrames[frame], drawPos, angle, scale * m_castingProgress, WHITE);
	}
	else
	{
		Vector2 halfSize = Vector2{ (float)m_animationFrames[frame].width / 2 * scale, (float)m_animationFrames[frame].width / 2 * scale };
		Vector2 rotated = Vector2Rotate(halfSize, angle);

		Vector2 drawPos = Vector2{ position.x - camPos.x, position.y - camPos.y };
		drawPos = Vector2Subtract(drawPos, rotated);

		DrawTextureEx(m_animationFrames[frame], drawPos, angle, scale, WHITE);
	}
}

bool FireBall::Casting() { return m_casting; }

void FireBall::Fire()
{
	m_casting = false;

	float angle = Vector2Angle(Vector2{ 0, 0 }, Vector2{ m_direction.x, m_direction.y });
	Vector2 offset = Vector2Rotate(Vector2{ castOffSet, 0 }, angle);
	position = position + v2(offset.x, offset.y);
}

void FireBall::UpdateCastingInfo(v2 position, v2 direction, float progress)
{
	this->position = position;
	m_direction = direction != v2() ? direction : v2(1, 0);
	m_castingProgress = progress;
}