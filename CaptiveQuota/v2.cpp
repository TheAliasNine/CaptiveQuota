#include "v2.h"

#include <math.h>

v2::v2()
{
	x = 0;
	y = 0;
}

v2::v2(float allVals)
{
	x = allVals;
	y = allVals;
}

v2::v2(float x, float y)
{
	this->x = x;
	this->y = y;
}



float v2::Magnitude()
{
	return sqrt(x * x + y * y);
}


v2 v2::Normalized()
{
	float mag = Magnitude();
	if (mag == 0)
		return v2();
	return v2(x / mag, y / mag);
}


float v2::Dot(v2 lhs, v2 rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

float v2::Dot(v2 other)
{
	return x * other.x + y * other.y;
}


float v2::CrossZ(v2 lhs, v2 rhs)
{
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

float v2::CrossZ(v2 other)
{
	return x * other.y - y * other.x;
}


v2 v2::ClampMag(v2 v, float max)
{
	float magSqrd = v.MagSqrd();
	if (max * max >= magSqrd) return v;

	float mag = sqrt(magSqrd);
	return v2(v.x / mag * max, v.y / mag * max);
}

v2 v2::ClampMag(float max)
{
	float magSqrd = MagSqrd();
	if (max * max >= magSqrd) return v2(x, y);

	float mag = sqrt(magSqrd);
	return v2(x / mag * max, y / mag * max);
}


v2 v2::ClampVector(v2 value, v2 min, v2 max)
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

v2 v2::ClampVector(v2 min, v2 max)
{
	v2 value = v2();
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

v2 v2::RotateVector(float radians)
{
	v2 value = v2(
		cos(radians) * x + sin(radians) * y,
		-sin(radians) * x + cos(radians) * y);
	return value;
}