#include "Map.h"

#include "raylib.h"


Map::Map()
{
	m_seed = 0;
	m_size = intV2();
	m_tiles = nullptr;

	m_exit = intV2();
	m_playerSpawn = intV2();
	m_cellMax = intV2();
	m_cellMin = intV2();
	m_portal = intV2();

	for (int i = 0; i < leverRoomCount; i++)
	{
		m_leverActive[i] = false;
	}
}

#pragma region Rule of Five
Map::~Map()
{
	delete[] m_tiles;
}

Map::Map(const Map& other)
{
	m_seed = other.m_seed;
	m_size = other.m_size;

	m_exit = other.m_exit;
	m_playerSpawn = other.m_playerSpawn;
	m_cellMax = other.m_cellMax;
	m_cellMin = other.m_cellMin;
	m_portal = other.m_portal;

	m_tiles = new Tile[m_size.x * m_size.y];
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		m_tiles[i] = other.m_tiles[i];
	}
}

Map& Map::operator= (const Map& other)
{
	m_seed = other.m_seed;
	m_size = other.m_size;

	m_exit = other.m_exit;
	m_playerSpawn = other.m_playerSpawn;
	m_cellMax = other.m_cellMax;
	m_cellMin = other.m_cellMin;
	m_portal = other.m_portal;

	m_tiles = new Tile[m_size.x * m_size.y];
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		m_tiles[i] = other.m_tiles[i];
	}
	return*this;
}

Map::Map(Map&& other)
{
	m_seed = other.m_seed;
	m_size = other.m_size;

	m_exit = other.m_exit;
	m_playerSpawn = other.m_playerSpawn;
	m_cellMax = other.m_cellMax;
	m_cellMin = other.m_cellMin;
	m_portal = other.m_portal;

	m_tiles = new Tile[m_size.x * m_size.y];
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		m_tiles[i] = other.m_tiles[i];
	}
}

Map& Map::operator= (Map&& other)
{
	m_seed = other.m_seed;
	m_size = other.m_size;

	m_exit = other.m_exit;
	m_playerSpawn = other.m_playerSpawn;
	m_cellMax = other.m_cellMax;
	m_cellMin = other.m_cellMin;
	m_portal = other.m_portal;

	m_tiles = new Tile[m_size.x * m_size.y];
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		m_tiles[i] = other.m_tiles[i];
	}
	return*this;
}

#pragma endregion

Map::Tile Map::operator[] (int index) { return m_tiles[index]; }
Map::Tile Map::operator[] (intV2 pos) { return m_tiles[pos.x + pos.y * m_size.x]; }

void Map::CreateMap(unsigned int seed)
{
	m_seed = seed;
	MapMaker mapMaker = MapMaker(this, m_seed);
	mapMaker.MakeMap();
}

void Map::DrawTiles(v2 camPos)
{

	for (int y = 0; y < m_size.y; y++)
	{
		for (int x = 0; x < m_size.x; x++)
		{
			Color color = Color();

			switch (m_tiles[x + y * m_size.x])
			{
			case Map::Tile::path:
				color = Color{ 68, 67, 65, 255 };
				break;
			case Map::Tile::wall:
				color = Color{ 42, 40, 39, 255 };
				break;
			case Map::Tile::exit:
				color = RED;
				//color = Color{ 80, 57, 55, 255 };
				break;
			case Map::Tile::vault:
				color = Color{ 63, 62, 85, 255 };
				break;
			case Map::Tile::portal:
				color = BLUE;
				break;
			case Map::Tile::cell:
				color = color = Color{ 78, 76, 55, 255 };
				break;
			}
			DrawRectangle(m_cellSize * x - camPos.x, m_cellSize * y - camPos.y, m_cellSize, m_cellSize, color);
		}
	}

	for (int i = 0; i < keyRoomCount; i++)
	{
		DrawCircle(m_cellSize * m_keyMolds[i].x - camPos.x, m_cellSize * m_keyMolds[i].y - camPos.y, m_cellSize, RED);
	}
	for (int i = 0; i < leverRoomCount; i++)
	{
		DrawCircle(m_cellSize * m_levers[i].x - camPos.x, m_cellSize * m_levers[i].y - camPos.y, m_cellSize, BLUE);
	}
}


intV2 Map::LeverPos(int index) { return m_levers[index]; }

void Map::ActivateLever(intV2 position)
{
	for (int i = 0; i < leverRoomCount; i++)
	{
		if (position != m_levers[i]) continue;
		if (!m_leverActive[i])
		{
			m_activeLeverCount++;
			m_leverActive[i] = true;
		}
		break;
	}
}

void Map::DeactivateLever(intV2 position)
{
	for (int i = 0; i < leverRoomCount; i++)
	{
		if (position != m_levers[i]) continue;
		if (m_leverActive[i])
		{
			m_activeLeverCount--;
			m_leverActive[i] = false;
		}
		break;
	}
}

bool Map::IsPortalActive()
{
	return m_activeLeverCount >= leverRoomCount;
}


int Map::CellSize() { return m_cellSize; }
void Map::SetCellSize(int size) { m_cellSize = size; }