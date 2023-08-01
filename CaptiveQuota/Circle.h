#pragma once

#include "Collider.h"

class Circle : public Collider
{
public:

	v2 position;
	float radius;

	bool CheckCollision(v2 point);
	bool CheckCollision(AABB* aabb, CollisionInfo* outInfo);
	bool CheckCollision(Circle* circle, CollisionInfo* outInfo);
	void Draw(Color color);
};