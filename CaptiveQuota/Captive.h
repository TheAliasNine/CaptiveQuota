 #pragma once

#include "AABB.h"
#include "AStarPathFinder.h"
#include "CaptivePathGoalFinding.h"
#include "HitBoxObject.h"
#include "Player.h"
#include "Map.h"

#include "raylib.h"

#include <vector>

class Captive : public HitBoxObject
{
	static const float c_scale;

	bool m_alive;

	Texture2D m_txtrAlive;
	Texture2D m_txtrDead;
	bool m_haveKey;

	void Discover();
	void DiscoverTile(intV2 tile);

	static const float c_speed;

public:
	static const float c_detectionRange;
	Player* p_player;
	Map* p_map;
	intV2 m_goal;
	bool* m_knownTiles;
	bool* m_checkedDiscovered;
	bool* m_undiscoveredArea;
	intV2 m_pathfindingGoal;

	AStarPath m_path;

	AStarPathFinder* p_pathfinder = nullptr;
	CaptivePathGoalFinding* p_goalFinder = nullptr;



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

	void SetAI(AStarPathFinder* pathfinding, CaptivePathGoalFinding* decisionMaking)
	{
		p_pathfinder = pathfinding;
		p_goalFinder = decisionMaking;
	}

};