#pragma once

#include "HitBoxObject.h"

#include "AABB.h"

#include "raylib.h"

class Captive : public HitBoxObject
{
	static const float scale;

	bool m_alive;

	Texture2D m_txtrAlive;
	Texture2D m_txtrDead;

public:
	Captive();
	Captive(v2 position);
	~Captive();
	Captive(const Captive& other);
	Captive& operator= (const Captive& other);
	Captive(Captive&& other);
	Captive& operator= (Captive&& other);


	void Update(float deltaTime);
	void Draw(v2 camPos);

	void Kill();
	bool Alive() { return m_alive; }
};