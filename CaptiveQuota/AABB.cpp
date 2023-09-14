#include "AABB.h"

#include "Circle.h"
#include "raylib.h"

bool AABB::CheckCollision(v2 point)
{
	return !(point.x > Max().x || point.y > Max().y || point.x < Min().x || point.y < Min().y);
}

bool AABB::CheckCollision(AABB* aabb, CollisionInfo* outInfo)
{
	bool collided = !(Max().x <= aabb->Min().x || aabb->Max().x <= Min().x || Max().y <= aabb->Min().y || aabb->Max().y <= Min().y);
	if (!collided || outInfo == nullptr) return collided;

	CollisionInfo info = CollisionInfo();
	info.collided = true;
	
	//find overlap dimensions
	v2 overlap = v2((Max().x < aabb->Max().x ? Max().x : aabb->Max().x) - (Min().x > aabb->Min().x ? Min().x : aabb->Min().x), 
		(Max().y < aabb->Max().y ? Max().y : aabb->Max().y) - (Min().y > aabb->Min().y ? Min().y : aabb->Min().y));

	//direction is from the centre to the short direction
	//depth is how much how much the overlap was for shorter
	if (overlap.x > overlap.y)
	{
		if (Max().y < aabb->Max().y) info.direction.y = -1;
		else info.direction.y = 1;
		info.depth = overlap.y;
	}
	else if (overlap.y > overlap.x)
	{
		if (Max().x < aabb->Max().x) info.direction.x = -1;
		else info.direction.x = 1;
		info.depth = overlap.x;
	}
	else //if tied the direction is to the corner and the depth is the magnitude of a vector of the overlap
	{
		if (Max().x < aabb->Max().x) info.direction.x = -1;
		else info.direction.x = 1;
		if (Max().y < aabb->Max().y) info.direction.y = -1;
		else info.direction.y = 1;
		info.direction = info.direction.Normalized();
		info.depth = overlap.Magnitude();
	}

	*outInfo = info;
	return collided;
}

bool AABB::CheckCollision(Circle* circle, CollisionInfo* outInfo)
{
	v2 closestPoint = v2::ClampVector(circle->position, Min(), Max());
	closestPoint.x -= circle->position.x;
	closestPoint.y -= circle->position.y;
	return circle->radius * circle->radius > closestPoint.x * closestPoint.x + closestPoint.y * closestPoint.y;
}

void AABB::Draw(Color color)
{
	DrawRectangle(Min().x, Min().y, size.x, size.y, color);
}