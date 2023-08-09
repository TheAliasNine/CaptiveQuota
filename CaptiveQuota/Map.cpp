#include "Map.h"


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
	MapMaker mapMaker = MapMaker(this, seed);
	mapMaker.MakeMap();
}