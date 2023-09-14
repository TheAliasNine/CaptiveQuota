#pragma once


struct intV2
{
	int x;
	int y;

	bool operator== (intV2 v) { return x == v.x && y == v.y; }
	bool operator!= (intV2 v) { return x != v.x || y != v.y; }
	intV2 operator+ (intV2 v) { return intV2{ x + v.x, y + v.y }; }
	intV2 operator- (intV2 v) { return intV2{ x - v.x, y - v.y }; }
	intV2 operator* (int i) { return intV2{ x * i, y * i }; }
};