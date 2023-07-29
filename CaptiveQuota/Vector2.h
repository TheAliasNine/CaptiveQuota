#pragma once

struct Vector2
{
public:
	float x;
	float y;

	//initializers
	Vector2();
	Vector2(float allVals);
	Vector2(float x, float y);


	//methods
	float Magnitude();
	float MagSqrd() { return x * x + y * y; }

	Vector2 Normalized();

	static float Dot(Vector2 lhs, Vector2 rhs);
	float Dot(Vector2 other);

	static float CrossZ(Vector2 lhs, Vector2 rhs);
	float CrossZ(Vector2 other);

	static Vector2 ClampMag(Vector2 v2, float max);
	Vector2 ClampMag(float max);

	static Vector2 ClampVector(Vector2 value, Vector2 min, Vector2 max);
	Vector2 ClampVector(Vector2 min, Vector2 max);


	//operators

	//ints
	friend Vector2 operator* (Vector2 v2, int i) { return Vector2(v2.x * i, v2.y * i); }
	friend Vector2 operator* (int i, Vector2 v2) { return Vector2(v2.x * i, v2.y * i); }
	Vector2 operator*= (int i) { return Vector2(x * i, y * i); }

	friend Vector2 operator/ (Vector2 v2, int i) { return Vector2(v2.x / i, v2.y / i); }
	friend Vector2 operator/ (int i, Vector2 v2) { return Vector2(v2.x / i, v2.y / i); }
	Vector2 operator/= (int i) { return Vector2(x / i, y / i); }


	//floats
	friend Vector2 operator* (Vector2 v2, float f) { return Vector2(v2.x * f, v2.y * f); }
	friend Vector2 operator* (float f, Vector2 v2) { return Vector2(v2.x * f, v2.y * f); }
	Vector2 operator*= (float f) { return Vector2(x * f, y * f); }

	friend Vector2 operator/ (Vector2 v2, float f) { return Vector2(v2.x / f, v2.y / f); }
	friend Vector2 operator/ (float f, Vector2 v2) { return Vector2(v2.x / f, v2.y / f); }
	Vector2 operator/= (float f) { return Vector2(x * f, y * f); }


	//Vector2
	Vector2 operator+ (Vector2 v2) { return Vector2(x + v2.x, y + v2.y); }
	Vector2 operator+= (Vector2 v2) { return Vector2(x + v2.x, y + v2.y); }
	Vector2 operator- (Vector2 v2) { return Vector2(x - v2.x, y - v2.y); }
	Vector2 operator-= (Vector2 v2) { return Vector2(x - v2.x, y - v2.y); }
	bool operator== (Vector2 v2) { return x == x && y == y; }
	bool operator!= (Vector2 v2) { return x != x || y != y; }
};