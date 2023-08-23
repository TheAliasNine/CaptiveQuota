#pragma once
#include "BoxObject.h"

#include "raylib.h"

class Player : public BoxObject
{

	static const float speed;
	static const float scale;

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

	void Move(float deltaTime);


public:
	Player();
	void Update(float deltaTime);

	void Draw();

	
};