#pragma once

#include "Collider.h"

class AABB : public Collider
{
public:

	Vector2 position;
	Vector2 size;

	Vector2 Min() { return position - size / 2; }
	Vector2 Max() { return position + size / 2; }


	bool CheckCollision(Vector2 point);
	bool CheckCollision(AABB* aabb, CollisionInfo* outInfo);
	bool CheckCollision(Circle* circle, CollisionInfo* outInfo);
};