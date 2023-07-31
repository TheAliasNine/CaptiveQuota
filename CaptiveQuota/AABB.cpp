#include "AABB.h"

#include "Circle.h"

bool AABB::CheckCollision(Vector2 point)
{
	return !(point.x > Max().x || point.y > Max().y || point.x < Min().x || point.y < Min().y);
}

bool AABB::CheckCollision(AABB* aabb, CollisionInfo* outInfo)
{
	return !(Max().x < aabb->Min().x || aabb->Max().x < Min().x || Max().y < aabb->Min().y || aabb->Max().y < Min().y);
}

bool AABB::CheckCollision(Circle* circle, CollisionInfo* outInfo)
{
	Vector2 closestPoint = Vector2::ClampVector(circle->position, Min(), Max());
	return circle->radius * circle->radius > closestPoint.x * closestPoint.x + closestPoint.y * closestPoint.y;
}