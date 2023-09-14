#pragma once

#include "v2.h"

struct CollisionInfo
{
	bool collided = false;
	v2 direction;
	float depth;

	void Invert()
	{
		if (!collided) return;
		direction.x *= -1;
		direction.y *= -1;
	}
};