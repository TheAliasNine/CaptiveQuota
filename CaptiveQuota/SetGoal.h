#pragma once

#include "Decision.h"
#include "Captive.h"

class SetGoal : public Decision<Captive>
{
public:
	virtual void MakeDecision(Captive& captive) override
	{
		Map map = *captive.p_map;
		if (!captive.HaveKey())
		{
			for (int i = 0; i < map.KeyMakerCount(); i++)
			{
				if (!map.CheckLineOfSight(captive.position, map.NodeToVector2(map.KeyMakerPos(i)), -1))
					continue;
				captive.m_goal = map.KeyMakerPos(i);
				return;
			}
		}
		for (int i = 0; i < map.LeverCount(); i++)
		{
			if (!map.CheckLineOfSight(captive.position, map.NodeToVector2(map.LeverPos(i)), -1))
				continue;
			captive.m_goal = map.LeverPos(i);
			return;
		}
	}
};