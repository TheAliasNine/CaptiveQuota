#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"
#include "KnowDoor.h"
#include "LeversActive.h"

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
		m_decA = new KnowDoor();
		m_decB = new LeversActive();
	}
};