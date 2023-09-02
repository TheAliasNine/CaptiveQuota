#pragma once

#include "Decision.h"
#include "Captive.h"

class Explore : public Decision<Captive>
{
public:
	virtual void MakeDecision(Captive& captive) override
	{

	}
};