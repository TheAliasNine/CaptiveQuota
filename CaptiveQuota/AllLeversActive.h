#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"

class AllLeversActive : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		
	}
public:
	AllLeversActive()
	{

	}
};