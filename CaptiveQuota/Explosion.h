#pragma once

#include "HitBoxObject.h"

#include "raylib.h"

class Explosion : public HitBoxObject
{
	static const int frameCount = 10;
	Texture2D m_animFrames[frameCount];
	static const float liveTime;
	static const float scale;

	float m_timer;
public:

	void Update(float deltaTime);
	void Draw(v2 camPos);

	Explosion();
	~Explosion();
	Explosion(const Explosion& other);
	Explosion& operator= (const Explosion& other);
	Explosion(const Explosion&& other);
	Explosion& operator= (const Explosion&& other);


	void Init(v2 position);

	bool m_alive = false;
};