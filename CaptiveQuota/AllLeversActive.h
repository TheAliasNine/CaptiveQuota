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
		
	}
public:
	AllLeversActive()
	{
		m_decA = new KnowPortalLocation();
		m_decB = new KnowLastLever();
	}
};