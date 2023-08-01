#pragma once

struct v2
{
public:
	float x;
	float y;

	//initializers
	v2();
	v2(float allVals);
	v2(float x, float y);


	//methods
	float Magnitude();
	float MagSqrd() { return x * x + y * y; }

	v2 Normalized();

	static float Dot(v2 lhs, v2 rhs);
	float Dot(v2 other);

	static float CrossZ(v2 lhs, v2 rhs);
	float CrossZ(v2 other);

	static v2 ClampMag(v2 v2, float max);
	v2 ClampMag(float max);

	static v2 ClampVector(v2 value, v2 min, v2 max);
	v2 ClampVector(v2 min, v2 max);

	v2 RotateVector(float radians);


	//operators

	//ints
	friend v2 operator* (v2 v, int i) { return v2(v.x * i, v.y * i); }
	friend v2 operator* (int i, v2 v) { return v2(v.x * i, v.y * i); }
	v2 operator*= (int i) { return v2(x * i, y * i); }

	friend v2 operator/ (v2 v, int i) { return v2(v.x / i, v.y / i); }
	friend v2 operator/ (int i, v2 v) { return v2(v.x / i, v.y / i); }
	v2 operator/= (int i) { return v2(x / i, y / i); }


	//floats
	friend v2 operator* (v2 v, float f) { return v2(v.x * f, v.y * f); }
	friend v2 operator* (float f, v2 v) { return v2(v.x * f, v.y * f); }
	v2 operator*= (float f) { return v2(x * f, y * f); }

	friend v2 operator/ (v2 v, float f) { return v2(v.x / f, v.y / f); }
	friend v2 operator/ (float f, v2 v) { return v2(v.x / f, v.y / f); }
	v2 operator/= (float f) { return v2(x * f, y * f); }


	//v2
	v2 operator+ (v2 v) { return v2(x + v.x, y + v.y); }
	v2 operator+= (v2 v) { return v2(x + v.x, y + v.y); }
	v2 operator- (v2 v) { return v2(x - v.x, y - v.y); }
	v2 operator-= (v2 v) { return v2(x - v.x, y - v.y); }
	bool operator== (v2 v) { return x == x && y == y; }
	bool operator!= (v2 v) { return x != x || y != y; }
};