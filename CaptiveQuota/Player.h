#pragma once
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

	Direction direction;

	Texture2D textures[static_cast<int>(Direction::DIRECTIONCOUNT)];

	void Move(float deltaTime);

public:
	void Update(float deltaTime);

	void Draw();
};