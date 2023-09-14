#pragma once
//base classes
#include "HitBoxObject.h"

#include "Map.h"

#include "v2.h"

#include "raylib.h"

class Exit : public HitBoxObject
{
private:
	static const float scale;

	Map* m_map;
	static const int textureCount = 2;
	Texture2D m_textures[textureCount];
public:
	Exit();
	Exit(Map* map);
	~Exit();
	Exit(const Exit& other);
	Exit& operator= (const Exit& other);
	Exit(Exit&& other);
	Exit& operator= (Exit&& other);

	void Update(float deltaTime) {}
	void Draw(v2 camPos);
};