#include "Minimap.h"

#include "raylib.h"

Minimap::Minimap()
{
	m_traversibleGrid = nullptr;
	m_discovered = nullptr;

	m_knownMin = intV2();
	m_knownMax = intV2();
	m_mapSize = intV2();
}

#pragma region Rule of Five
Minimap::~Minimap()
{
	delete[] m_traversibleGrid;
	delete[] m_discovered;
}

Minimap::Minimap(const Minimap& other)
{
	m_mapSize = other.m_mapSize;


	m_traversibleGrid = new bool[m_mapSize.x * m_mapSize.y];
	m_discovered = new bool[m_mapSize.x * m_mapSize.y];
	for (int i = 0; i < m_mapSize.x * m_mapSize.y; i++)
	{
		m_traversibleGrid[i] = other.m_traversibleGrid;
		m_discovered[i] = other.m_discovered[i];
	}


	m_knownMin = other.m_knownMin;
	m_knownMax = other.m_knownMax;
}

Minimap& Minimap::operator= (const Minimap& other)
{
	m_mapSize = other.m_mapSize;


	m_traversibleGrid = new bool[m_mapSize.x * m_mapSize.y];
	m_discovered = new bool[m_mapSize.x * m_mapSize.y];
	for (int i = 0; i < m_mapSize.x * m_mapSize.y; i++)
	{
		m_traversibleGrid[i] = other.m_traversibleGrid;
		m_discovered[i] = other.m_discovered[i];
	}


	m_knownMin = other.m_knownMin;
	m_knownMax = other.m_knownMax;

	return *this;
}

Minimap::Minimap(Minimap&& other)
{
	m_mapSize = other.m_mapSize;


	m_traversibleGrid = other.m_traversibleGrid;
	other.m_traversibleGrid = nullptr;

	m_discovered = other.m_discovered;
	other.m_discovered = nullptr;


	m_knownMin = other.m_knownMin;
	m_knownMax = other.m_knownMax;
}

Minimap& Minimap::operator= (Minimap&& other)
{
	m_mapSize = other.m_mapSize;


	m_traversibleGrid = other.m_traversibleGrid;
	other.m_traversibleGrid = nullptr;

	m_discovered = other.m_discovered;
	other.m_discovered = nullptr;


	m_knownMin = other.m_knownMin;
	m_knownMax = other.m_knownMax;

	return *this;
}
#pragma endregion

void Minimap::LoadMap(Map& map)
{
	delete[] m_traversibleGrid;
	delete[] m_discovered;

	m_mapSize = map.Size();
	m_knownMin = map.PlayerSpawn();
	m_knownMax = map.PlayerSpawn();
	
	m_traversibleGrid = new bool[m_mapSize.x * m_mapSize.y];
	m_discovered = new bool[m_mapSize.x * m_mapSize.y];
	for (int i = 0; i < m_mapSize.x * m_mapSize.y; i++)
	{
		m_traversibleGrid[i] = map[i] != Map::Tile::wall;
		m_discovered[i] = false;
	}

	DiscoverTile(map.PlayerSpawn());
}

void Minimap::DiscoverTile(intV2 pos)
{
	m_discovered[pos.x + pos.y * m_mapSize.x] = true;
	m_discovered[pos.x - 1 + pos.y * m_mapSize.x] = true;
	m_discovered[pos.x + 1 + pos.y * m_mapSize.x] = true;
	m_discovered[pos.x + (pos.y + 1) * m_mapSize.x] = true;
	m_discovered[pos.x - 1 + (pos.y + 1) * m_mapSize.x] = true;
	m_discovered[pos.x + 1 + (pos.y + 1) * m_mapSize.x] = true;
	m_discovered[pos.x + (pos.y - 1) * m_mapSize.x] = true;
	m_discovered[pos.x - 1 + (pos.y - 1) * m_mapSize.x] = true;
	m_discovered[pos.x + 1 + (pos.y - 1) * m_mapSize.x] = true;
}


void Minimap::Draw()
{
	int miniX = WINDOWX - miniMapSize - spacing;
	int miniY = WINDOWY - miniMapSize - spacing;
	DrawRectangle(miniX, miniY, miniMapSize, miniMapSize, Color{ 0,0,0,128 });

	intV2 mapRange = m_knownMax - m_knownMin;
	mapRange.x += 1;
	mapRange.y += 1;
	int cellsize = 10;
	
	for (int y = m_knownMin.y; y <= m_knownMax.y; y++)
	{
		for (int x = m_knownMin.x; x <= m_knownMax.x; x++)
		{
			if (m_discovered[x + y * m_mapSize.x] && m_traversibleGrid[x + y * m_mapSize.x])
				DrawRectangle((x - m_knownMin.x) * cellsize + miniX, (y - m_knownMin.y) * cellsize + miniY, cellsize, cellsize, WHITE);
		}
	}
}