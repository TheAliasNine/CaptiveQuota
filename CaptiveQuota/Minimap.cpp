#include "Minimap.h"

#include <raylib.h>
#include <math.h>

Minimap::Minimap()
{
	m_traversibleGrid = nullptr;
	m_discovered = nullptr;

	m_knownMin = intV2();
	m_knownMax = intV2();
	m_mapSize = intV2();


	m_mapImage = Image();
	m_imageData = new unsigned short int[miniMapSize * miniMapSize];
	for (int i = 0; i < miniMapSize * miniMapSize; i++)
	{
		m_imageData[i] = 0b1000000000000000;
	}
	m_mapImage.data = m_imageData;
	m_mapImage.width = miniMapSize;
	m_mapImage.height = miniMapSize;
	m_mapImage.format = PixelFormat::UNCOMPRESSED_GRAY_ALPHA;
	m_mapImage.mipmaps = 1;
	m_mapTexture = LoadTextureFromImage(m_mapImage);
}

#pragma region Rule of Five
Minimap::~Minimap()
{
	delete[] m_traversibleGrid;
	delete[] m_discovered;
	UnloadImage(m_mapImage);
	UnloadTexture(m_mapTexture);
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
	
	m_imageData = new unsigned short int[miniMapSize * miniMapSize];
	for (int i = 0; i < miniMapSize * miniMapSize; i++)
	{
		m_imageData[i] = other.m_imageData[i];
	}
	m_mapImage = other.m_mapImage;
	m_mapImage.data = m_imageData;

	m_mapTexture = LoadTextureFromImage(m_mapImage);
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

	UnloadImage(m_mapImage);
	UnloadTexture(m_mapTexture);

	m_imageData = new unsigned short int[miniMapSize * miniMapSize];
	for (int i = 0; i < miniMapSize * miniMapSize; i++)
	{
		m_imageData[i] = other.m_imageData[i];
	}
	m_mapImage = other.m_mapImage;
	m_mapImage.data = m_imageData;

	m_mapTexture = LoadTextureFromImage(m_mapImage);

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

	UnloadImage(m_mapImage);
	UnloadTexture(m_mapTexture);

	m_imageData = new unsigned short int[miniMapSize * miniMapSize];
	for (int i = 0; i < miniMapSize * miniMapSize; i++)
	{
		m_imageData[i] = other.m_imageData[i];
	}
	m_mapImage = other.m_mapImage;
	m_mapImage.data = m_imageData;

	m_mapTexture = LoadTextureFromImage(m_mapImage);
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

	UnloadImage(m_mapImage);
	UnloadTexture(m_mapTexture);

	m_imageData = new unsigned short int[miniMapSize * miniMapSize];
	for (int i = 0; i < miniMapSize * miniMapSize; i++)
	{
		m_imageData[i] = other.m_imageData[i];
	}
	m_mapImage = other.m_mapImage;
	m_mapImage.data = m_imageData;

	m_mapTexture = LoadTextureFromImage(m_mapImage);

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
		m_traversibleGrid[i] = map[i] != Map::Tile::wall && map[i] != Map::Tile::null;
		m_discovered[i] = false;
	}

	DiscoverTile(map.PlayerSpawn());
}

void Minimap::DiscoverTile(intV2 pos)
{
	bool changed = false;
	prevMin = m_knownMin;
	prevMax = m_knownMax;

	//go out left
	intV2 tilePos = pos;

	int lMax = 0;
	int rMax = 0;

	intV2 check = tilePos;
	changed = Discover(check) || changed;
	check.x--;
	while (m_traversibleGrid[check.x + check.y * m_mapSize.x] && lMax < c_detectionRange)
	{
		changed = Discover(check) || changed;
		check.x--;
		lMax++;
	}

	check = tilePos;
	check.x++;
	while (m_traversibleGrid[check.x + check.y * m_mapSize.x] && rMax < c_detectionRange)
	{
		changed = Discover(check) || changed;
		check.x++;
		rMax++;
	}

	//up
	check = tilePos;
	check.y--;
	int tlMax = lMax;
	int trMax = rMax;
	for (int top = 0; top < c_detectionRange && m_traversibleGrid[check.x + check.y * m_mapSize.x]; top++, check.y--)
	{
		changed = Discover(check) || changed;

		check.x = tilePos.x;
		check.x--;
		for (int topLeft = 0; topLeft < tlMax && m_traversibleGrid[check.x + check.y * m_mapSize.x]; topLeft++, check.x--)
		{
			changed = Discover(check) || changed;
		}
		tlMax = abs(check.x - tilePos.x) < tlMax ? abs(check.x - tilePos.x ) : tlMax;

		check.x = tilePos.x;
		check.x++;
		for (int topRight = 0; topRight < trMax && m_traversibleGrid[check.x + check.y * m_mapSize.x]; topRight++, check.x++)
		{
			changed = Discover(check) || changed;
		}
		trMax = abs(check.x - tilePos.x) < trMax ? abs(check.x - tilePos.x) : trMax;
		check.x = tilePos.x;
	}

	//down
	check = tilePos;
	check.y++;
	int dlMax = lMax;
	int drMax = rMax;
	for (int down = 0; down < c_detectionRange && m_traversibleGrid[check.x + check.y * m_mapSize.x]; down++, check.y++)
	{
		changed = Discover(check) || changed;

		check.x = tilePos.x;
		check.x--;
		for (int downLeft = 0; downLeft < dlMax && m_traversibleGrid[check.x + check.y * m_mapSize.x]; downLeft++, check.x--)
		{
			changed = Discover(check) || changed;
		}
		dlMax = abs(check.x - tilePos.x) < dlMax ? abs(check.x - tilePos.x) : dlMax;

		check.x = tilePos.x;
		check.x++;
		for (int downRight = 0; downRight < drMax && m_traversibleGrid[check.x + check.y * m_mapSize.x]; downRight++, check.x++)
		{
			changed = Discover(check) || changed;
		}
		drMax = abs(check.x - tilePos.x) < drMax ? abs(check.x - tilePos.x) : drMax;
		check.x = tilePos.x;
	}

	if (!changed) return;
	UnloadTexture(m_mapTexture);


	//only need to redraw from scratch if the knownsize changes
	//otherwise can just draw in the new 

	if (m_knownMax != prevMax || m_knownMin != prevMin) 
	{
		for (int i = 0; i < miniMapSize * miniMapSize; i++) //reset map image
		{
			m_imageData[i] = 0b1000000000000000;
		}
	}
	
	intV2 mapRange = m_knownMax - m_knownMin;
	mapRange.x += 1;
	mapRange.y += 1;
	int cellsize = mapRange.x > mapRange.y ? miniMapSize / mapRange.x : miniMapSize / mapRange.y;
	if (cellsize > maxCellSize) cellsize = maxCellSize;
	if (cellsize < minCellSize) cellsize = minCellSize;

	int offset = (miniMapSize - (cellsize * mapRange.x)) / 2 + (miniMapSize - (cellsize * mapRange.y)) / 2 * miniMapSize;

	for (int y = m_knownMin.y; y <= m_knownMax.y; y++)
	{
		for (int x = m_knownMin.x; x <= m_knownMax.x; x++)
		{
			if (!m_discovered[x + y * m_mapSize.x] || !m_traversibleGrid[x + y * m_mapSize.x]) continue;

			for (int pixY = 0; pixY < cellsize; pixY++)
			{
				for (int pixX = 0; pixX < cellsize; pixX++)
				{
					m_imageData[((x - m_knownMin.x) + (y - m_knownMin.y) * (miniMapSize)) * cellsize + (pixX + pixY * miniMapSize) + offset] = 0b1111111111111111;
				}
			}
		}
	}

	m_mapTexture = LoadTextureFromImage(m_mapImage);
}

bool Minimap::Discover(intV2 discoverPos)
{
	if (discoverPos.x < 0 || discoverPos.x >= m_mapSize.x || discoverPos.y < 0 || discoverPos.y >= m_mapSize.y)
		return false;
	int index = discoverPos.x + discoverPos.y * m_mapSize.x;

	if (m_discovered[index]) return false;
	m_discovered[index] = true;

	if (discoverPos.x < m_knownMin.x) m_knownMin.x = discoverPos.x - 1;
	if (discoverPos.y < m_knownMin.y) m_knownMin.y = discoverPos.y - 1;
	if (discoverPos.x > m_knownMax.x) m_knownMax.x = discoverPos.x + 1;
	if (discoverPos.y > m_knownMax.y) m_knownMax.y = discoverPos.y + 1;

	return true;
}

void Minimap::Draw(intV2 playerPos)
{
	int miniMapX = WINDOWX - miniMapSize - spacingFromEdge;
	int miniMapY = WINDOWY - miniMapSize - spacingFromEdge;
	//draw map
	DrawTexture(m_mapTexture, miniMapX, miniMapY, WHITE);
	
	//draw player
	if (playerPos.x < m_knownMin.x) playerPos.x = m_knownMin.x;
	if (playerPos.x > m_knownMax.x) playerPos.x = m_knownMax.x;
	if (playerPos.y < m_knownMin.y) playerPos.y = m_knownMin.y;
	if (playerPos.y > m_knownMax.y) playerPos.y = m_knownMax.y;
	intV2 mapRange = m_knownMax - m_knownMin;
	mapRange.x += 1;
	mapRange.y += 1;
	int cellsize = mapRange.x > mapRange.y ? miniMapSize / mapRange.x : miniMapSize / mapRange.y;
	if (cellsize > maxCellSize) cellsize = maxCellSize;
	if (cellsize < minCellSize) cellsize = minCellSize;
	int offset = (miniMapSize - (cellsize * mapRange.x)) / 2 + (miniMapSize - (cellsize * mapRange.y)) / 2 * miniMapSize;

	int posX = (miniMapSize - (cellsize * mapRange.x)) / 2 + (playerPos.x - m_knownMin.x) * cellsize + cellsize / 2;
	int posY = (miniMapSize - (cellsize * mapRange.y)) / 2 + (playerPos.y - m_knownMin.y) * cellsize + cellsize / 2;
	DrawCircle(miniMapX + posX, miniMapY + posY, playerSize, PURPLE);
}