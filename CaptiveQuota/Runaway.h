#pragma once

#include "Decision.h"

#include "Captive.h"

class RunAway : public Decision<Captive>
{
	void MakeDecision(Captive& captive) override;
};