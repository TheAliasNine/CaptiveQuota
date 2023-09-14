#pragma once
#include "HitBoxObject.h"

#include "raylib.h"

class Player : public HitBoxObject
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
	~Player();
	Player(const Player& other);
	Player& operator= (const Player& other);
	Player(Player&& other);
	Player& operator= (Player&& other);

	void Update(float deltaTime);

	void Draw(v2 camPos);

	
};