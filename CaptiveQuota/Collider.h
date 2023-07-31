#pragma once

#include "CollisionInfo.h"

#include "Vector2.h"


class AABB;
class Circle;

class Collider
{
protected:

	virtual bool CheckCollision(Collider* other) { return other->CheckCollision(this); }
	virtual bool CheckCollision(Vector2 point, CollisionInfo* outInfo) = 0;
	virtual bool CheckCollision(AABB* other, CollisionInfo* outInfo) = 0;
	virtual bool CheckCollision(Circle* circle, CollisionInfo* outInfo) = 0;
};