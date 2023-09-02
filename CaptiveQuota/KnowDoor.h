#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"

class KnowDoor : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		
	}
public:
	KnowDoor()
	{

	}
};