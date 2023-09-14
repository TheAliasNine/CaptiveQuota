#pragma once

#include "Decision.h"
#include "Captive.h"

class SetGoal : public Decision<Captive>
{
public:
	virtual void MakeDecision(Captive& captive) override
	{
		Map* map = captive.p_map;
		if (map->IsPortalActive())
		{
			if (map->CheckLineOfSight(captive.position, map->NodeToVector2(map->PortalPosition()), captive.c_detectionRange * captive.p_map->CellSize()))
			{
				captive.m_goal = map->PortalPosition();
			}
		}
		if (!captive.HaveKey())
		{
			for (int i = 0; i < map->KeyMakerCount(); i++)
			{
				if (captive.m_goal == map->KeyMakerPos(i))
					return;
				if (!map->CheckLineOfSight(captive.position, map->NodeToVector2(map->KeyMakerPos(i)), captive.c_detectionRange * captive.p_map->CellSize()))
					continue;
				captive.m_goal = map->KeyMakerPos(i);
				return;
			}
		}
		for (int i = 0; i < map->LeverCount(); i++)
		{
			if (captive.m_goal == map->LeverPos(i))
				return;
			if (map->IsLeverActive(map->LeverPos(i)))
				continue;
			if (!map->CheckLineOfSight(captive.position, map->NodeToVector2(map->LeverPos(i)), captive.c_detectionRange * captive.p_map->CellSize()))
				continue;
			captive.m_goal = map->LeverPos(i);
			return;
		}

	}
};