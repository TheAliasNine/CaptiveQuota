#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"

class KnowPortalLocation : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{

	}
public:
	KnowPortalLocation()
	{

	}
};