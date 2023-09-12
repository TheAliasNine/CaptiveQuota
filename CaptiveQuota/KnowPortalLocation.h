#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"
#include "GoToPortal.h"
#include "Explore.h"

class KnowPortalLocation : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		return captive.m_knownTiles[captive.p_map->Index(captive.p_map->PortalPosition())];
	}
public:
	KnowPortalLocation()
	{
		m_decA = new GoToPortal();
		m_decB = new Explore();
	}
};