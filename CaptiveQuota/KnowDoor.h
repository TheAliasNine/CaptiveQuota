#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"
#include "GoToDoor.h"
#include "Explore.h"

class KnowDoor : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		
	}
public:
	KnowDoor()
	{
		m_decA = new GoToDoor();
		m_decB = new Explore();
	}
};