#pragma once

#include "Decision.h"

#include "AStarPathFinder.h"
#include "Captive.h"

class RunAway : public Decision<Captive>
{
private:
	const int distance = 15;
	float DistanceSquared(intV2 a, intV2 b)
	{
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
	}

public:
	void MakeDecision(Captive& captive) override
	{
		AStarPathFinder pathfinder = AStarPathFinder(captive.p_map);

		pathfinder.Setup(captive.p_map->Vector2ToNode(captive.position), captive.p_map->Vector2ToNode(captive.p_player->position), -1);
		
		do
		{
			pathfinder.ProcessNextNode();

		} while (DistanceSquared(pathfinder.JustProcessedNode().Pos(), captive.p_map->Vector2ToNode(captive.p_player->position)) < distance * distance);
		captive.m_goal = pathfinder.JustProcessedNode().Pos();
	}
};