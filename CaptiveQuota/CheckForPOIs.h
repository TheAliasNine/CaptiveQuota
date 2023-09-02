#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"
#include "Map.h"

class CheckForPOIs : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		Map map = *captive.mapRef;
		if (!captive.HaveKey())
		{
			for (int i = 0; i < map.KeyMakerCount(); i++)
			{
				if (map.CheckLineOfSight(captive.position, map.NodeToVector2(map.KeyMakerPos(i)), -1))
					return true;
			}
		}
		for (int i = 0; i < map.LeverCount(); i++)
		{
			if (map.CheckLineOfSight(captive.position, map.NodeToVector2(map.LeverPos(i)), -1))
				return true;
		}
		return false;
	}
public:
	CheckForPOIs()
	{
		//ToDo: Make branches 
	}
};