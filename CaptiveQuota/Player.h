#pragma once
#include "AABB.h"
#include "SceneObject.h"

#include "raylib.h"

class Player : public SceneObject
{

	static const float speed;

	enum class Direction
	{
		down,
		up,
		left,
		right,
		DIRECTIONCOUNT
	};

	Direction m_direction;

	Texture2D m_textures[static_cast<int>(Direction::DIRECTIONCOUNT)];
	AABB m_hitbox;

	void Move(float deltaTime);


public:
	Player();
	void Update(float deltaTime);

	void Draw();

	AABB Hitbox() { return m_hitbox; }
};