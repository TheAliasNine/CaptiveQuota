#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"

class KnowLastLever : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{

	}
public:
	KnowLastLever()
	{

	}
};