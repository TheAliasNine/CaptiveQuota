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
		if (captive.m_running)
			return true;
		return captive.p_map->CheckLineOfSight(captive.position, captive.p_player->position, captive.c_detectionRange * captive.p_map->CellSize());;
	}
public:
	SeePlayer()
	{
		m_decA = new RunAway();
		m_decB = new CheckForPOIs();
	}

};