#pragma once

#include "Collider.h"

class Circle : public Collider
{
public:

	float radius;

	bool CheckCollision(Collider* other, CollisionInfo* outInfo)
	{
		bool collided = other->CheckCollision(this, outInfo);
		if (outInfo != nullptr)
			outInfo->Invert();
		return collided;
	}
	bool CheckCollision(v2 point);
	bool CheckCollision(AABB* aabb, CollisionInfo* outInfo);
	bool CheckCollision(Circle* circle, CollisionInfo* outInfo);
	void Draw(Color color);
};