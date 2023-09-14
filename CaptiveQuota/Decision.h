#pragma once

template <typename Actor>
class Decision
{
protected:

public:
	virtual void MakeDecision(Actor& actor) = 0;
};