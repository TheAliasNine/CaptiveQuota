#pragma once

#include "Collider.h"

class AABB : public Collider
{
public:

	v2 size;

	v2 Min() { return position - (size / 2); }
	v2 Max() { return position + (size / 2); }


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