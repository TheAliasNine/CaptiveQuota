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

	void Draw();

private:

	static const int miniMapSize = 150;
	static const int spacing = 25;
	static const int maxCellSize = 15;

	intV2 m_mapSize;
	intV2 m_knownMin;
	intV2 m_knownMax;

	bool* m_traversibleGrid;
	bool* m_discovered;
};