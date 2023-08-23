#pragma once

#include "Map.h"

#include "intV2.h"

class Minimap
{
public:
	Minimap();
	~Minimap();

	Minimap(const Minimap& other);
	Minimap& operator= (const Minimap& other);
	Minimap(Minimap&& other);
	Minimap& operator= (Minimap&& other);

	void LoadMap(Map& map);

	void DiscoverTile(intV2 position);

	void Draw(intV2 playerPos);

private:

	static const int miniMapSize = 150;
	static const int spacingFromEdge = 25;
	static const int minCellSize = 2;
	static const int maxCellSize = 10;
	static const int playerSize = 4;

	intV2 m_mapSize;
	intV2 m_knownMin;
	intV2 m_knownMax;
	unsigned short int* m_imageData;
	Image m_mapImage;
	Texture2D m_mapTexture;

	bool* m_traversibleGrid;
	bool* m_discovered;
};