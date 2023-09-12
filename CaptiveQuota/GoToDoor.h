#pragma once

#include "Decision.h"
#include "Captive.h"

class GoToDoor : public Decision<Captive>
{
public:
	virtual void MakeDecision(Captive& captive) override
	{
		captive.m_goal = captive.p_map->Exit();
	}
};