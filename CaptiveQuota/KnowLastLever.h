#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"
#include "GoToLever.h"
#include "Explore.h"

class KnowLastLever : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		for (int i = 0; i < captive.p_map->LeverCount(); i++)
		{
			if (captive.p_map->IsLeverActive(captive.p_map->LeverPos(i)))
				continue;
			if (captive.m_knownTiles[captive.p_map->Index(captive.p_map->LeverPos(i))])
				return true;
		}
		return false;
	}
public:
	KnowLastLever()
	{
		m_decA = new GoToLever();
		m_decB = new Explore();
	}
};