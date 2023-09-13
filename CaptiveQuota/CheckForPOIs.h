#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"
#include "Map.h"
#include "SetGoal.h"
#include "HaveKey.h"

class CheckForPOIs : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		Map* map = captive.p_map;
		if (!captive.HaveKey())
		{
			for (int i = 0; i < map->KeyMakerCount(); i++)
			{
				if (captive.m_goal == map->KeyMakerPos(i))
					return true;
				if (map->CheckLineOfSight(captive.position, map->NodeToVector2(map->KeyMakerPos(i)), captive.c_detectionRange * captive.p_map->CellSize()))
					return true;
			}
		}
		for (int i = 0; i < map->LeverCount(); i++)
		{
			if (map->IsLeverActive(map->LeverPos(i)))
				continue;
			if (captive.m_goal == map->LeverPos(i))
				return true;
			if (map->CheckLineOfSight(captive.position, map->NodeToVector2(map->LeverPos(i)), captive.c_detectionRange * captive.p_map->CellSize()))
				return true;
		}
		return false;
	}
public:
	CheckForPOIs()
	{
		m_decA = new SetGoal();
		m_decB = new HaveKey();
	}
};