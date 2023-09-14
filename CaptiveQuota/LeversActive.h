#pragma once


#include "ABDecisionNode.h"
#include "Captive.h"
#include "Explore.h"
#include "AllLeversActive.h"

class LeversActive : public ABDecisionNode<Captive>
{
protected:
	const int minimum = 2;
	virtual bool Condition(Captive& captive) override
	{
		int count = 0;
		for (int i = 0; i < captive.p_map->LeverCount(); i++)
		{
			if (captive.p_map->IsLeverActive(captive.p_map->LeverPos(i)))
				count++;
		}
		if (count >= minimum)
			return true;
		return false;
	}
public:
	LeversActive()
	{
		m_decA = new AllLeversActive();
		m_decB = new Explore();
	}
};