#pragma once

#include "Collider.h"

class Circle : public Collider
{
public:

	Vector2 position;
	float radius;

	bool CheckCollision(Vector2 point);
	bool CheckCollision(AABB* aabb, CollisionInfo* outInfo);
	bool CheckCollision(Circle* circle, CollisionInfo* outInfo);
};