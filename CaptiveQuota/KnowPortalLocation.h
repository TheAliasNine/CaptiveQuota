#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"
#include "GoToPortal.h"
#include "Explore.h"

class KnowPortalLocation : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{

	}
public:
	KnowPortalLocation()
	{
		m_decA = new GoToPortal();
		m_decB = new Explore();
	}
};