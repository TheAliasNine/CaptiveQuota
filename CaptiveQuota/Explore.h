#pragma once

#include "Decision.h"
#include "Captive.h"

class Explore : public Decision<Captive>
{
	float DistanceSquared(intV2 a, intV2 b)
	{
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
	}
public:
	virtual void MakeDecision(Captive& captive) override
	{
		intV2 mapSize = captive.p_map->Size();
		float closestDistance = 0;
		intV2 closest = intV2();

		intV2 playerPos = captive.p_map->Vector2ToNode(captive.position);

		for (int y = 0; y < mapSize.y; y++)
		{
			for (int x = 0; x < mapSize.x; x++)
			{
				intV2 node = intV2{ x, y };
				int index = x + y * mapSize.x;
				if (!captive.m_undiscoveredArea[index])
					continue;

				float distance = DistanceSquared(node, playerPos);
				if (distance < closestDistance)
				{
					closest = node;
					closestDistance = distance;
				}
			}
		}

		if (!captive.m_undiscoveredArea[0] && closest == intV2())
			closest = playerPos;

		captive.m_goal = closest;
	}
};