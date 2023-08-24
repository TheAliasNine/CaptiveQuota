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
	Map map;
	Minimap minimap;
	
	v2 camPos = v2();

	Player player;
	std::list<FireBall> fireballs;
	
	Texture2D background;

	void PhysicStep();

	bool CheckMapCollisions(HitBoxObject* obj, bool resolve);

	void DrawUI();

	void ShootFireBall();

public:

	Game();
	~Game();

	void Update(float deltaTime);
	void Draw();
};