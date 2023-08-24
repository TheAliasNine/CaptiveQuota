#pragma once

#include "HitBoxObject.h"

#include "Raylib.h"

class FireBall : public HitBoxObject
{
private:
	static const float speed;
	static const float animationTime;
	static const int frameCount = 2;
	static const float scale;

	Texture2D m_animationFrames[frameCount];
	v2 m_direction;
	float m_timer = 0;

public:
	FireBall() = default;
	FireBall(v2 position, v2 direction);
	~FireBall();
	FireBall(const FireBall& other);
	FireBall& operator= (const FireBall& other);
	FireBall(FireBall&& other);
	FireBall& operator= (FireBall&& other);

	void Draw(v2 camPos);
	void Update(float deltaTime);
};