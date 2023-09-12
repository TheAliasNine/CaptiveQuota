#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"
#include "KnowPortalLocation.h"
#include "KnowLastLever.h"

class AllLeversActive : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		for (int i = 0; i < captive.p_map->LeverCount(); i++)
		{
			if (!captive.p_map->IsLeverActive(captive.p_map->LeverPos(i)))
				return false;
		}
		return true;
	}
public:
	AllLeversActive()
	{
		m_decA = new KnowPortalLocation();
		m_decB = new KnowLastLever();
	}
};