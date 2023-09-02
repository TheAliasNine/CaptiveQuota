#pragma once

#include "AABB.h"
#include "HitBoxObject.h"
#include "Player.h"
#include "Map.h"

#include "raylib.h"

class Captive : public HitBoxObject
{
	static const float scale;

	bool m_alive;

	Texture2D m_txtrAlive;
	Texture2D m_txtrDead;
	bool m_haveKey;

public:
	static const float detectionRange;
	Player* playerRef;
	Map* mapRef;
	intV2 m_goal;

	Captive();
	Captive(v2 position, Player* player, Map* map);
	~Captive();
	Captive(const Captive& other);
	Captive& operator= (const Captive& other);
	Captive(Captive&& other);
	Captive& operator= (Captive&& other);


	void Update(float deltaTime);
	void Draw(v2 camPos);

	void Kill();
	bool Alive() { return m_alive; }

	bool HaveKey() { return m_haveKey; }
};