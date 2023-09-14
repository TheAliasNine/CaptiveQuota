#pragma once

#include "HitBoxObject.h"

#include "Map.h"

#include "raylib.h"

class KeyMaker : public HitBoxObject
{
	static const float scale;

	Map* m_map;

	static const int textureCount = 1;
	Texture2D m_textures[textureCount];

	Sound m_interacted;

	intV2 m_keyMakerPos = intV2();

public:
	KeyMaker();
	KeyMaker(Map* map, int leverInd);

	~KeyMaker();
	KeyMaker(const KeyMaker& other);
	KeyMaker& operator= (const KeyMaker& other);
	KeyMaker(KeyMaker&& other);
	KeyMaker& operator= (KeyMaker&& other);

	void Update(float deltaTime) {}
	void Draw(v2 camPos);

	void Interact();
};