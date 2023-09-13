#pragma once

#include "Decision.h"
#include "Captive.h"

#include <iostream>

class GoToLever : public Decision<Captive>
{
public:
	virtual void MakeDecision(Captive& captive) override
	{
		for (int i = 0; i < captive.p_map->LeverCount(); i++)
		{
			if (captive.p_map->IsLeverActive(captive.p_map->LeverPos(i)))
				continue;
			captive.m_goal = captive.p_map->LeverPos(i);
			return;
		}
		std::cout << "Going to lever." << std::endl;
	}
};