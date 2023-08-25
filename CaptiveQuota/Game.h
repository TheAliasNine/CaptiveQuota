#pragma once

#include "AppState.h"
#include "FireBall.h"
#include "Map.h"
#include "Minimap.h"
#include "Player.h"

#include "v2.h"

#include "raylib.h"

#include <list>

class Game : public AppState
{
	Map m_map;
	Minimap m_minimap;
	
	v2 m_camPos = v2();

	Player m_player;
	std::list<FireBall> m_fireballs;

	Sound m_fireballcast;

	static const float castTime;
	bool m_casting = false;
	float m_castingTimer = 0;


	
	Texture2D m_background;

	void PhysicStep();

	bool CheckMapCollisions(HitBoxObject* obj, bool resolve);

	void DrawUI();

	void CastFireBall();
	void ShootFireBall();

	void CreateExplosion();

public:

	Game();
	~Game();

	void Update(float deltaTime);
	void Draw();
};