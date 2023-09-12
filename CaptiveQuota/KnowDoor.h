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
		return captive.m_knownTiles[captive.p_map->Index(captive.p_map->Exit())];
	}
public:
	KnowDoor()
	{
		m_decA = new GoToDoor();
		m_decB = new Explore();
	}
};