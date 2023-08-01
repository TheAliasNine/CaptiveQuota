#include "AABB.h"

#include "Circle.h"
#include "raylib.h"

bool AABB::CheckCollision(v2 point)
{
	return !(point.x > Max().x || point.y > Max().y || point.x < Min().x || point.y < Min().y);
}

bool AABB::CheckCollision(AABB* aabb, CollisionInfo* outInfo)
{
	return !(Max().x < aabb->Min().x || aabb->Max().x < Min().x || Max().y < aabb->Min().y || aabb->Max().y < Min().y);
}

bool AABB::CheckCollision(Circle* circle, CollisionInfo* outInfo)
{
	v2 closestPoint = v2::ClampVector(circle->position, Min(), Max());
	return circle->radius * circle->radius > closestPoint.x * closestPoint.x + closestPoint.y * closestPoint.y;
}

void AABB::Draw(Color color)
{
	DrawRectangle(Min().x, Min().y, size.x, size.y, color);
}