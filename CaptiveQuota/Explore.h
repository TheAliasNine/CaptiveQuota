#pragma once

#include "Decision.h"
#include "Captive.h"

class Explore : public Decision<Captive>
{
	const int immediateRange = 13;
	const int nearRange = 30;
	float DistanceSquared(intV2 a, intV2 b)
	{
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
	}
public:
	virtual void MakeDecision(Captive& captive) override
	{
		intV2 mapSize = captive.p_map->Size();
		intV2 playerPos = captive.p_map->Vector2ToNode(captive.position);


		if (captive.m_undiscoveredArea[captive.p_map->Index(captive.m_goal)])
			return;
		std::vector<intV2> immediateAreas = std::vector<intV2>();
		immediateAreas.reserve(16);
		
		std::vector<intV2> nearAreas = std::vector<intV2>();
		nearAreas.reserve(16);
		std::vector<intV2> farAreas = std::vector<intV2>();
		farAreas.reserve(16);

		for (int y = 0; y < mapSize.y; y++)
		{
			for (int x = 0; x < mapSize.x; x++)
			{
				intV2 node = intV2{ x, y };
				int index = x + y * mapSize.x;
				if (!captive.m_undiscoveredArea[index])
					continue;
				if (DistanceSquared(captive.p_map->Vector2ToNode(captive.position), node) > immediateRange * immediateRange)
				{
					immediateAreas.push_back(node);
				}
				else if (DistanceSquared(captive.p_map->Vector2ToNode(captive.position), node) > nearRange * nearRange)
				{
					nearAreas.push_back(node);
				}
				else
				{
					farAreas.push_back(node);
				}
			}
		}

		if (immediateAreas.size() == 0)
			if (nearAreas.size() == 0)
				if (farAreas.size() == 0)
					captive.m_goal = playerPos;
				else
					captive.m_goal = farAreas[std::rand() % farAreas.size()];
			else
				captive.m_goal = nearAreas[std::rand() % nearAreas.size()];
		else
			captive.m_goal = immediateAreas[std::rand() % immediateAreas.size()];
	}
};