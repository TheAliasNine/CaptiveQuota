#pragma once

#include "HitBoxObject.h"

#include "Map.h"

#include "raylib.h"

class Lever : public HitBoxObject
{
	static const float scale;
	static const float speed;
	static const float border;

	Map* m_map;

	static const int textureCount = 2;
	Texture2D m_textures[textureCount];

	Sound m_leverOn;
	Sound m_leverOff;

	float m_handlePos = 0;
	intV2 m_leverPos = intV2();

public:
	Lever();
	Lever(Map* map, int leverInd);

	~Lever();
	Lever(const Lever& other);
	Lever& operator= (const Lever& other);
	Lever(Lever&& other);
	Lever& operator= (Lever&& other);

	void Update(float deltaTime);
	void Draw(v2 camPos);

	void Interact();
	void TurnOn();
	void TurnOff();
};