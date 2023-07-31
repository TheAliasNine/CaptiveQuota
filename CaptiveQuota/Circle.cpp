#include "Circle.h"

#include "AABB.h"

bool Circle::CheckCollision(Vector2 point)
{
	float distSqr = (point.x - position.x) * (point.x - position.x) + (point.y - position.y) * (point.y - position.y);
	
	return distSqr < radius * radius;
}

bool Circle::CheckCollision(AABB* aabb, CollisionInfo* outInfo)
{
	Vector2 closestPoint = Vector2::ClampVector(position, aabb->Min(), aabb->Max());

	return radius * radius > closestPoint.x * closestPoint.x + closestPoint.y * closestPoint.y;
}

bool Circle::CheckCollision(Circle* circle, CollisionInfo* outInfo)
{
	return (position, circle->position).MagSqrd() < (radius + circle->radius) * (radius + circle->radius);
}