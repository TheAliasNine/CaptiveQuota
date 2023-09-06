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

	}
public:
	KnowLastLever()
	{
		m_decA = new GoToLever();
		m_decB = new Explore();
	}
};