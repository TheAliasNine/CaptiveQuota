#pragma once

#include "ABDecisionNode.h"
#include "Captive.h"
#include "Runaway.h"
#include "CheckForPOIs.h"

class SeePlayer : public ABDecisionNode<Captive>
{
protected:
	virtual bool Condition(Captive& captive) override
	{
		return captive.mapRef->CheckLineOfSight(captive.position, captive.playerRef->position, captive.detectionRange);
	}
public:
	SeePlayer()
	{
		m_decA = new RunAway();
		m_decB = new CheckForPOIs();
	}

};