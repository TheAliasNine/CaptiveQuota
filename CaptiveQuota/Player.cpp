#include "Player.h"

#include "InputManager.h"
#include "v2.h"

const float Player::speed = 350;

void Player::Update(float deltaTime)
{
	Move(deltaTime);
}

void Player::Move(float deltaTime)
{
	v2 moveDir = v2();
	if (InputManager::KeyDown(InputManager::Up))
		moveDir.y -= 1;
	if (InputManager::KeyDown(InputManager::Down))
		moveDir.y += 1;
	if (InputManager::KeyDown(InputManager::Left))
		moveDir.x -= 1;
	if (InputManager::KeyDown(InputManager::Right))
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

	position = position + (moveDir * speed * deltaTime);
}

void Player::Draw()
{
	int playerHeight = 100;
	int playerWidth = 50;
	DrawRectangle(WINDOWX / 2 - (playerWidth / 2), WINDOWY / 2 - (playerHeight / 2), playerWidth, playerHeight, WHITE);
	//DrawTexture(textures[static_cast<int>(direction)], )
}