#include "Vector2.h"

#include <math.h>

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(float allVals)
{
	x = allVals;
	y = allVals;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}



float Vector2::Magnitude()
{
	return sqrt(x * x + y * y);
}


Vector2 Vector2::Normalized()
{
	float mag = Magnitude();
	return Vector2(x / mag, y / mag);
}


float Vector2::Dot(Vector2 lhs, Vector2 rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

float Vector2::Dot(Vector2 other)
{
	return x * other.x + y * other.y;
}


float Vector2::CrossZ(Vector2 lhs, Vector2 rhs)
{
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

float Vector2::CrossZ(Vector2 other)
{
	return x * other.y - y * other.x;
}


Vector2 Vector2::ClampMag(Vector2 v2, float max)
{
	float magSqrd = v2.MagSqrd();
	if (max * max >= magSqrd) return v2;

	float mag = sqrt(magSqrd);
	return Vector2(v2.x / mag * max, v2.y / mag * max);
}

Vector2 Vector2::ClampMag(float max)
{
	float magSqrd = MagSqrd();
	if (max * max >= magSqrd) return Vector2(x, y);

	float mag = sqrt(magSqrd);
	return Vector2(x / mag * max, y / mag * max);
}


Vector2 Vector2::ClampVector(Vector2 value, Vector2 min, Vector2 max)
{
	if (value.x < min.x)
	{
		value.x = min.x;
	}
	else if (value.x > max.x)
	{
		value.x = max.x;
	}
	if (value.y < min.y)
	{
		value.y = min.y;
	}
	else if (value.y > max.y)
	{
		value.y = max.y;
	}
	return value;
}

void Vector2::ClampVector(Vector2 min, Vector2 max)
{
	if (x < min.x)
	{
		x = min.x;
	}
	else if (x > max.x)
	{
		x = max.x;
	}
	if (y < min.y)
	{
		y = min.y;
	}
	else if (y > max.y)
	{
		y = max.y;
	}
}