#pragma once

#include "CollisionInfo.h"

#include "v2.h"


class AABB;
class Circle;
struct Color;

class Collider
{
protected:

public:
	v2 position;

	virtual bool CheckCollision(Collider* other, CollisionInfo* outInfo)
	{
		bool collided = other->CheckCollision(this, outInfo);
		if (outInfo != nullptr)
			outInfo->Invert();
		return collided;
	}
	virtual bool CheckCollision(v2 point) = 0;
	virtual bool CheckCollision(AABB* other, CollisionInfo* outInfo) = 0;
	virtual bool CheckCollision(Circle* circle, CollisionInfo* outInfo) = 0;
	virtual void Draw(Color color) = 0;
};