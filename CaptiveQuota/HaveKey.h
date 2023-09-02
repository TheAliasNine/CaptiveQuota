#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"

class HaveKey : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		return captive.HaveKey();
	}

public:
	HaveKey()
	{
		
	}
};