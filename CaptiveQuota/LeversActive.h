#pragma once


#include "ABDecisionNode.h"
#include "Captive.h"

class LeversActive : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		return false;
	}
public:
	LeversActive()
	{

	}
};