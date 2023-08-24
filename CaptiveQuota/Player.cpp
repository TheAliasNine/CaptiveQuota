#include "Player.h"

#include "AABB.h"
#include "InputManager.h"
#include "v2.h"

const float Player::speed = 350;
const float Player::scale = 0.5f;

Player::Player()
{
	m_direction = Direction::down;
	m_textures[static_cast<int>(Direction::up)] = LoadTexture("Assets\\Images\\Player_Up.png");
	m_textures[static_cast<int>(Direction::down)] = LoadTexture("Assets\\Images\\Player_Down.png");
	m_textures[static_cast<int>(Direction::left)] = LoadTexture("Assets\\Images\\Player_Left.png");
	m_textures[static_cast<int>(Direction::right)] = LoadTexture("Assets\\Images\\Player_Right.png");

	AABB* aabb = new AABB();
	aabb->size = v2((m_textures[0].width - 60) * scale, (m_textures[0].height / 2) * scale);
	m_hitbox = aabb;
}
#pragma region Rule of 5
Player::~Player()
{
	for (int i = 0; i < static_cast<int>(Direction::DIRECTIONCOUNT); i++)
	{
		UnloadTexture(m_textures[i]);
	}
}

Player::Player(const Player& other)
{
	m_textures[static_cast<int>(Direction::up)] = LoadTexture("Assets\\Images\\Player_Up.png");
	m_textures[static_cast<int>(Direction::down)] = LoadTexture("Assets\\Images\\Player_Down.png");
	m_textures[static_cast<int>(Direction::left)] = LoadTexture("Assets\\Images\\Player_Left.png");
	m_textures[static_cast<int>(Direction::right)] = LoadTexture("Assets\\Images\\Player_Right.png");

	m_direction = other.m_direction;
	m_hitbox = new AABB(*static_cast<AABB*>(other.m_hitbox));
}

Player& Player::operator= (const Player& other)
{
	m_direction = other.m_direction;
	m_hitbox = new AABB(*static_cast<AABB*>(other.m_hitbox));
	return *this;
}
Player::Player(Player&& other)
{
	for (int i = 0; i < static_cast<int>(Direction::DIRECTIONCOUNT); i++)
	{
		m_textures[i] = other.m_textures[i];
		other.m_textures[i] = Texture2D();
	}

	m_direction = other.m_direction;
	m_hitbox = new AABB(*static_cast<AABB*>(other.m_hitbox));
}
Player& Player::operator= (Player&& other)
{
	m_direction = other.m_direction;
	m_hitbox = new AABB(*static_cast<AABB*>(other.m_hitbox));
	return *this;
}

#pragma endregion
void Player::Update(float deltaTime)
{
	Move(deltaTime);

	m_hitbox->position = v2(position.x, position.y + (m_textures[0].height / 2) * scale / 2);
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


	if (moveDir.y < 0)
		m_direction = Direction::up;
	else if (moveDir.y > 0)
		m_direction = Direction::down;

	if (moveDir.x < 0)
		m_direction = Direction::left;
	else if (moveDir.x > 0)
		m_direction = Direction::right;


	position = position + (moveDir * speed * deltaTime);
}

void Player::Draw(v2 camPos)
{
	float playerWidth = m_textures[static_cast<int>(m_direction)].width * scale;
	float playerHeight = m_textures[static_cast<int>(m_direction)].height * scale;

	Vector2 texturePos = Vector2{ float(WINDOWX / 2 - (playerWidth / 2)), float(WINDOWY / 2 - (playerHeight / 2)) };
	DrawTextureEx(m_textures[static_cast<int>(m_direction)], texturePos, 0, scale, Color{ 255, 255, 255, 255 });
}