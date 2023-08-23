#pragma once

#include "v2.h"

struct CollisionInfo
{
	bool collided = false;
	v2 direction;
	float depth;
};