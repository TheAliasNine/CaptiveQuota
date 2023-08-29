#include "Circle.h"

#include "AABB.h"
#include "raylib.h"

bool Circle::CheckCollision(v2 point)
{
	float distSqr = (point.x - position.x) * (point.x - position.x) + (point.y - position.y) * (point.y - position.y);
	
	return distSqr < radius * radius;
}

bool Circle::CheckCollision(AABB* aabb, CollisionInfo* outInfo)
{
	v2 closestPoint = v2::ClampVector(position, aabb->Min(), aabb->Max());
	closestPoint.x -= position.x;
	closestPoint.y -= position.y;

	return radius * radius > closestPoint.x * closestPoint.x + closestPoint.y * closestPoint.y;
}

bool Circle::CheckCollision(Circle* circle, CollisionInfo* outInfo)
{
	return (position, circle->position).MagSqrd() < (radius + circle->radius) * (radius + circle->radius);
}

void Circle::Draw(Color color)
{
	DrawCircle(position.x, position.y, radius, color);
}