#pragma once

#include "Captive.h"
#include "SeePlayer.h"

class CaptivePathGoalFinding
{
	//Root node of the decision tree
	SeePlayer rootNode = SeePlayer();
public:
	void FindGoal(Captive& captive) { rootNode.MakeDecision(captive); }
};