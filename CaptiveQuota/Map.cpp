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
	
	for (int i = 0; i < static_cast<int>(Tile::TILECOUNT); i++)
	{
		m_textures[i] = Texture2D();
	}
	m_textures[static_cast<int>(Tile::cell)] = LoadTexture("Assets\\Images\\Cell.png");
	m_textures[static_cast<int>(Tile::exit)] = LoadTexture("Assets\\Images\\Exit.png");
	m_textures[static_cast<int>(Tile::exitRoom)] = LoadTexture("Assets\\Images\\ExitRoom.png");
	m_textures[static_cast<int>(Tile::null)] = Texture2D();
	m_textures[static_cast<int>(Tile::path)] = LoadTexture("Assets\\Images\\Path.png");
	m_textures[static_cast<int>(Tile::vault)] = LoadTexture("Assets\\Images\\Vault.png");
	m_textures[static_cast<int>(Tile::wall)] = LoadTexture("Assets\\Images\\Wall.png");
}

#pragma region Rule of Five
Map::~Map()
{
	for (int i = 0; i < static_cast<int>(Tile::TILECOUNT); i++)
	{
		UnloadTexture(m_textures[i]);
	}
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
	v2 centreScreen = camPos;
	centreScreen.x += WINDOWX / 2;
	centreScreen.y += WINDOWY / 2;
	intV2 centre = Vector2ToNode(centreScreen);
	int halfXNodes = WINDOWX / 2 / m_cellSize + 1;
	int halfYNodes = WINDOWY / 2 / m_cellSize + 1;
	intV2 lowestDraw = intV2{ centre.x - halfXNodes, centre.y - halfYNodes };
	intV2 highestDraw = intV2{ centre.x + halfXNodes + 1, centre.y + halfYNodes + 1 };

	if (highestDraw.x > m_size.x) highestDraw.x = m_size.x;
	if (highestDraw.y > m_size.y) highestDraw.y = m_size.y;
	if (lowestDraw.x < 0) lowestDraw.x = 0;
	if (lowestDraw.y < 0) lowestDraw.y = 0;

	for (int y = lowestDraw.y; y < highestDraw.y; y++)
	{
		for (int x = lowestDraw.x; x < highestDraw.x; x++)
		{
			Vector2 position = Vector2{ m_cellSize * x - camPos.x, m_cellSize * y - camPos.y };
			float scale = m_cellSize / (float)m_textures[static_cast<int>(m_tiles[x + y * m_size.x])].width;
			DrawTextureEx(m_textures[static_cast<int>(m_tiles[x + y * m_size.x])], position, 0, scale, WHITE);
		}
	}
	for (int i = 0; i < keyRoomCount; i++)
	{
		DrawCircle(m_cellSize * m_keyMolds[i].x - camPos.x + float (m_cellSize) / 2, m_cellSize * m_keyMolds[i].y - camPos.y + float(m_cellSize) / 2, (float)m_cellSize / 2, RED);
	}
	for (int i = 0; i < leverRoomCount; i++)
	{
		DrawCircle(m_cellSize * m_levers[i].x - camPos.x + float(m_cellSize) / 2, m_cellSize * m_levers[i].y - camPos.y + float(m_cellSize) / 2, (float)m_cellSize / 2, BLUE);
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

intV2 Map::Vector2ToNode(v2 vector)
{
	return intV2{ int(round(vector.x)) / m_cellSize, int(round(vector.y)) / m_cellSize };
}