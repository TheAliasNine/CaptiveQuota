#include "Player.h"

#include "InputManager.h"
#include "v2.h"

const float Player::speed = 40;

void Player::Update(float deltaTime)
{
	Move(deltaTime);
}

void Player::Move(float deltaTime)
{
	v2 moveDir = v2();
	if (InputManager::KeyPressed(InputManager::Up))
		moveDir.y -= 1;
	if (InputManager::KeyPressed(InputManager::Down))
		moveDir.y += 1;
	if (InputManager::KeyPressed(InputManager::Left))
		moveDir.x -= 1;
	if (InputManager::KeyPressed(InputManager::Right))
		moveDir.x += 1;

	moveDir = moveDir.Normalized();

	if (moveDir.x < 0)
		direction = Direction::left;
	else if (moveDir.x > 0)
		direction = Direction::right;

	if (moveDir.y < 0)
		direction = Direction::up;
	else if (moveDir.y > 0)
		direction = Direction::down;

	position += moveDir * speed;
}

void Player::Draw()
{
	//DrawTexture(textures[static_cast<int>(direction)], )
}