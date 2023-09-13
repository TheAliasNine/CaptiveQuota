#include "Captive.h"

#include "AABB.h"
#include "CaptivePathGoalFinding.h"

#include <math.h>

const float Captive::c_scale = 0.5f;
const float Captive::c_interactRange = 150;
const float Captive::c_speed = 200;
const float Captive::c_detectionRange = 15;

Captive::Captive()
{
	m_alive = false;
	m_knownTiles = nullptr;
	m_checkedDiscovered = nullptr;
	m_undiscoveredArea = nullptr;

	m_txtrAlive = LoadTexture("Assets\\Images\\Captive.png");
	m_txtrDead = LoadTexture("Assets\\Images\\CaptiveCorpse.png");
}
Captive::Captive(v2 position, Player* player, Map* map)
{
	this->position = position;
	m_alive = true;

	m_txtrAlive = LoadTexture("Assets\\Images\\Captive.png");
	m_txtrDead = LoadTexture("Assets\\Images\\CaptiveCorpse.png");

	AABB* hitbox = new AABB();
	hitbox->position = position;
	hitbox->size = v2(m_txtrAlive.width * c_scale, m_txtrAlive.height * c_scale / 2);

	m_hitbox = hitbox;

	p_player = player;
	p_map = map;
	
	intV2 size = p_map->Size();
	m_knownTiles = new bool[size.x * size.y];
	for (int i = 0; i < size.x * size.y; i++)
	{
		m_knownTiles[i] = false;
	}
	m_checkedDiscovered = new bool[size.x * size.y];
	for (int i = 0; i < size.x * size.y; i++)
	{
		m_checkedDiscovered[i] = false;
	}
	m_undiscoveredArea = new bool[size.x * size.y];
	for (int i = 0; i < size.x * size.y; i++)
	{
		m_undiscoveredArea[i] = false;
	}

	Discover();

	m_hitboxOffset = v2(0, (m_txtrAlive.height / 2) * c_scale / 2);
}
#pragma region Rule of 5
Captive::~Captive()
{
	UnloadTexture(m_txtrAlive);
	UnloadTexture(m_txtrDead);
	delete[] m_knownTiles;
	delete[] m_checkedDiscovered;
	delete[] m_undiscoveredArea;
}
Captive::Captive(const Captive& other)
{
	p_map = other.p_map;
	position = other.position;
	m_alive = other.m_alive;

	m_txtrAlive = LoadTexture("Assets\\Images\\Captive.png");
	m_txtrDead = LoadTexture("Assets\\Images\\CaptiveCorpse.png");

	m_hitbox = new AABB(*static_cast<AABB*>(other.m_hitbox));

	intV2 size = p_map->Size();
	m_knownTiles = new bool[size.x * size.y];
	for (int i = 0; i < size.x * size.y; i++)
	{
		m_knownTiles[i] = other.m_knownTiles[i];
	}
	m_checkedDiscovered = new bool[size.x * size.y];
	for (int i = 0; i < size.x * size.y; i++)
	{
		m_checkedDiscovered[i] = other.m_checkedDiscovered[i];
	}
	m_undiscoveredArea = new bool[size.x * size.y];
	for (int i = 0; i < size.x * size.y; i++)
	{
		m_undiscoveredArea[i] = other.m_undiscoveredArea[i];
	}
}

Captive& Captive::operator= (const Captive& other)
{
	p_map = other.p_map;
	position = other.position;
	m_alive = other.m_alive;

	m_hitbox = new AABB(*static_cast<AABB*>(other.m_hitbox));

	intV2 size = p_map->Size();
	for (int i = 0; i < size.x * size.y; i++)
	{
		m_knownTiles[i] = other.m_knownTiles[i];
	}
	for (int i = 0; i < size.x * size.y; i++)
	{
		m_checkedDiscovered[i] = other.m_checkedDiscovered[i];
	}
	for (int i = 0; i < size.x * size.y; i++)
	{
		m_undiscoveredArea[i] = other.m_undiscoveredArea[i];
	}

	return *this;
}
Captive::Captive(Captive&& other)
{
	position = other.position;
	m_alive = other.m_alive;

	m_txtrAlive = LoadTexture("Assets\\Images\\Captive.png");
	m_txtrDead = LoadTexture("Assets\\Images\\CaptiveCorpse.png");

	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;
	
	p_map = other.p_map;

	m_knownTiles = other.m_knownTiles;
	other.m_knownTiles = nullptr;
	m_checkedDiscovered = other.m_checkedDiscovered;
	other.m_checkedDiscovered = nullptr;
	m_undiscoveredArea = other.m_undiscoveredArea;
	other.m_undiscoveredArea = nullptr;
}
Captive& Captive::operator= (Captive&& other)
{
	position = other.position;
	m_alive = other.m_alive;

	m_hitbox = other.m_hitbox;
	other.m_hitbox = nullptr;

	p_map = other.p_map;

	m_knownTiles = other.m_knownTiles;
	other.m_knownTiles = nullptr;
	m_checkedDiscovered = other.m_checkedDiscovered;
	other.m_checkedDiscovered = nullptr;
	m_undiscoveredArea = other.m_undiscoveredArea;
	other.m_undiscoveredArea = nullptr;

	return *this;
}
#pragma endregion

#include <iostream>

void Captive::Update(float deltaTime)
{
	if (!m_alive || m_escaped) return;

	p_goalFinder->FindGoal(*this);

	if (m_path.Length() <= 0 || m_path.Goal() != m_goal)
	{
		std::cout << "Changing path to " << m_goal.x << ", " << m_goal.y << "." << std::endl;
		p_pathfinder->Setup(p_map->Vector2ToNode(m_hitbox->position), m_goal);

		m_path = p_pathfinder->FindPathToGoal();
	}

	if (m_path.Length() == 0)
		return;

	Discover();

	if (position + m_hitboxOffset == p_map->NodeToVector2(m_path.Current().Pos()))
	{
		m_path.Progress();
	}

	v2 posToNode = (p_map->NodeToVector2(m_path.Current().Pos()) - (position + m_hitboxOffset));
	v2 direction = posToNode.Normalized();

	if (posToNode.Magnitude() < c_speed * deltaTime)
	{
		position = p_map->NodeToVector2(m_path.Current().Pos()) - m_hitboxOffset;
		if (m_path.Current().Pos() == m_path.Goal())
			m_running = false;
	}
	else
		position = position + (c_speed * deltaTime * direction);
	m_hitbox->position = position + m_hitboxOffset;

	CheckPOIs();
}

void Captive::CheckPOIs()
{
	for (int i = 0; i < p_map->LeverCount(); i++)
	{
		v2 obj = p_map->NodeToVector2(p_map->LeverPos(i));

		float distanceSqrd = (obj.x - position.x) * (obj.x - position.x) + (obj.y - position.y) * (obj.y - position.y);

		if (distanceSqrd < c_interactRange * c_interactRange)
			p_map->ActivateLever(p_map->LeverPos(i));
	}
	if (!m_haveKey)
	{
		for (int i = 0; i < p_map->KeyMakerCount(); i++)
		{
			v2 obj = p_map->NodeToVector2(p_map->KeyMakerPos(i));

			float distanceSqrd = (obj.x - position.x) * (obj.x - position.x) + (obj.y - position.y) * (obj.y - position.y);

			if (distanceSqrd < c_interactRange * c_interactRange)
				m_haveKey = true;
		}
	}
}

void Captive::Draw(v2 camPos)
{
	if (m_escaped)
		return;
	Vector2 drawPos = Vector2{ position.x - camPos.x, position.y - camPos.y };
	if (m_alive)
	{
		drawPos.x -= m_txtrAlive.width / 2 * c_scale;
		drawPos.y -= m_txtrAlive.height / 2 * c_scale;
		DrawTextureEx(m_txtrAlive, drawPos, 0, c_scale, WHITE);
	}
	else
	{
		drawPos.x -= m_txtrDead.width / 2 * c_scale;
		drawPos.y -= m_txtrDead.height / 2 * c_scale;

		drawPos.y += m_txtrAlive.height / 2 - m_txtrDead.height;
		DrawTextureEx(m_txtrDead, drawPos, 0, c_scale, WHITE);
	}

}

void Captive::Kill()
{
	m_alive = false;
}

void Captive::Discover()
{
	//go out left
	intV2 tilePos = p_map->Vector2ToNode(position);

	int lMax = 0;
	int rMax = 0;

	intV2 check = tilePos;
	DiscoverTile(check);
	check.x--;
	while (Map::IsTraversible((*p_map)[check]) && lMax < c_detectionRange)
	{
		DiscoverTile(check);
		check.x--;
		lMax++;
	}
	
	check = tilePos;
	check.x++;
	while (Map::IsTraversible((*p_map)[check]) && rMax < c_detectionRange)
	{
		DiscoverTile(check);
		check.x++;
		rMax++;
	}

	//up
	check = tilePos;
	check.y--;
	int tlMax = lMax;
	int trMax = rMax;
	for (int top = 0; top < c_detectionRange && Map::IsTraversible((*p_map)[check]); top++, check.y--)
	{
		DiscoverTile(check);

		check.x = tilePos.x;
		check.x--;
		for (int topLeft = 0; topLeft < tlMax && Map::IsTraversible((*p_map)[check]); topLeft++, check.x--)
		{
			DiscoverTile(check);
		}
		tlMax = abs(check.x - tilePos.x) < tlMax ? abs(check.x - tilePos.x) : tlMax;

		check.x = tilePos.x;
		check.x++;
		for (int topRight = 0; topRight < trMax && Map::IsTraversible((*p_map)[check]); topRight++, check.x++)
		{
			DiscoverTile(check);
		}
		trMax = abs(check.x - tilePos.x) < trMax ? abs(check.x - tilePos.x) : trMax;
		check.x = tilePos.x;
	}

	//down
	check = tilePos;
	check.y++;
	int dlMax = lMax;
	int drMax = rMax;
	for (int down = 0; down < c_detectionRange && Map::IsTraversible((*p_map)[check]); down++, check.y++)
	{
		DiscoverTile(check);

		check.x = tilePos.x;
		check.x--;
		for (int downLeft = 0; downLeft < dlMax && Map::IsTraversible((*p_map)[check]); downLeft++, check.x--)
		{
			DiscoverTile(check);
		}
		dlMax = abs(check.x - tilePos.x) < dlMax ? abs(check.x - tilePos.x) : dlMax;
		if (check.x)

		check.x = tilePos.x;
		check.x++;
		for (int downRight = 0; downRight < drMax && Map::IsTraversible((*p_map)[check]); downRight++, check.x++)
		{
			DiscoverTile(check);
		}
		drMax = abs(check.x - tilePos.x) < drMax ? abs(check.x - tilePos.x) : drMax;
		check.x = tilePos.x;
	}
}

void Captive::DiscoverTile(intV2 tile)
{
	int index = p_map->Index(tile);
	if (m_checkedDiscovered[index]) return;
	m_undiscoveredArea[index] = false;
	m_checkedDiscovered[index] = true;

	m_knownTiles[p_map->Index(tile)] = true;

	for (int i = 0; i < 4; i++)
	{
		intV2 neighbour = tile;
		switch (i)
		{
		case 0:
			tile.x++;
			break;
		case 1:
			tile.x--;
			break;
		case 2:
			tile.y++;
			break;
		case 3:
			tile.y--;
			break;
		}

		if (neighbour.x < 0 || neighbour.x > p_map->Size().x || neighbour.y < 0 || neighbour.y > p_map->Size().y)
			continue;
		int neighbourIndex = p_map->Index(neighbour);
		if (m_knownTiles[neighbourIndex] || (*p_map)[neighbourIndex] == Map::Tile::wall || (*p_map)[neighbourIndex] == Map::Tile::exit)
			continue;
		m_undiscoveredArea[neighbourIndex] = true;
	}
}