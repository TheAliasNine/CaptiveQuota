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
		return captive.p_map->CheckLineOfSight(captive.position, captive.p_player->position, captive.c_detectionRange);
	}
public:
	SeePlayer()
	{
		m_decA = new RunAway();
		m_decB = new CheckForPOIs();
	}

};