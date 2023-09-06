#pragma once


#include "ABDecisionNode.h"
#include "Captive.h"
#include "Explore.h"
#include "AllLeversActive.h"

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
		m_decA = new AllLeversActive();
		m_decB = new Explore();
	}
};