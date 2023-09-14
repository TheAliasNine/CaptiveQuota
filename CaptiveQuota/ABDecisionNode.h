#pragma once

#include "Decision.h"

template <typename Actor>
class ABDecisionNode : public Decision<Actor>
{
protected:
	Decision<Actor>* m_decA = nullptr;
	Decision<Actor>* m_decB = nullptr;
	
	virtual bool Condition(Actor& actor) = 0;

	~ABDecisionNode()
	{
		delete m_decA;
		delete m_decB;
	}
	
public:
	virtual void MakeDecision(Actor& actor) override
	{
		if (Condition(actor))
			m_decA->MakeDecision(actor);
		else
			m_decB->MakeDecision(actor);
	}


};