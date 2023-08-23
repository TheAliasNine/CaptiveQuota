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
	virtual bool CheckCollision(Collider* other) { return other->CheckCollision(this); }
	virtual bool CheckCollision(v2 point) = 0;
	virtual bool CheckCollision(AABB* other, CollisionInfo* outInfo) = 0;
	virtual bool CheckCollision(Circle* circle, CollisionInfo* outInfo) = 0;
	virtual void Draw(Color color) = 0;
};