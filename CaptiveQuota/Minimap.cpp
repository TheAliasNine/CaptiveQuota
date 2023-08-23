#include "Minimap.h"

#include <raylib.h>

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
	intV2 prevMin = m_knownMin;
	intV2 prevMax = m_knownMax;

	for (int i = 0; i < 9; i++)
	{
		int index = 0;
		switch (i)
		{
		case(0):
			index = pos.x + pos.y * m_mapSize.x;
			break;
		case(1):
			index = pos.x - 1 + pos.y * m_mapSize.x;
			break;
		case(2):
			index = pos.x + 1 + pos.y * m_mapSize.x;
			break;
		case(3):
			index = pos.x + (pos.y + 1) * m_mapSize.x;
			break;
		case(4):
			index = pos.x - 1 + (pos.y + 1) * m_mapSize.x;
			break;
		case(5):
			index = pos.x + 1 + (pos.y + 1) * m_mapSize.x;
			break;
		case(6):
			index = pos.x + (pos.y - 1) * m_mapSize.x;
			break;
		case(7):
			index = pos.x - 1 + (pos.y - 1) * m_mapSize.x;
			break;
		case(8):
			index = pos.x + 1 + (pos.y - 1) * m_mapSize.x;
			break;
		}
		if (m_discovered[index]) continue;
		changed = true;
		m_discovered[index] = true;



		if (pos.x - 1 < m_knownMin.x) m_knownMin.x = pos.x - 1;
		if (pos.y - 1 < m_knownMin.y) m_knownMin.y = pos.y - 1;
		if (pos.x + 1 > m_knownMax.x) m_knownMax.x = pos.x + 1;
		if (pos.y + 1 > m_knownMax.y) m_knownMax.y = pos.y + 1;
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


void Minimap::Draw(intV2 playerPos)
{
	int miniMapX = WINDOWX - miniMapSize - spacingFromEdge;
	int miniMapY = WINDOWY - miniMapSize - spacingFromEdge;
	//draw map
	DrawTexture(m_mapTexture, miniMapX, miniMapY, WHITE);
	
	//draw player
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