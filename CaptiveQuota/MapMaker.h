#pragma once

#include "intV2.h"
#include "CorridorNode.h"

class MapMaker
{
public:
	enum class Tile
	{
		blank,
		path,
		wall,
		cell,
		exit,
		vault,
		lever,
		key,
	};
private:

//Map Making Constants---------------------------------------
	static const int majorRoomCount = 3;
	static const int leverRoomCount = 3;
	static const int keyRoomCount = 2;

	//the size around the centre e.g. 1 = 3x3, 2 = 5x5  (1 + 2x)
	static const int majorRoomSize = 5;
	static const int leverRoomSize = 3;
	static const int keyRoomSize = 3;

	static const int majorMaxDistance = 60;
	static const int majorMinDistance = 50;
	static const int minLeverRoomDist = 15;
	static const int minKeyRoomDist = 15;

//Setup------------------------------------------------------
	unsigned int seed = 0;

	/// <summary>
	/// places the 3 major rooms that also decides the size of the map
	/// </summary>
	void MajorRooms();

	/// <summary>
	/// places the 3 lever rooms required for opening the magic vault
	/// </summary>
	void LeverRooms();

	/// <summary>
	/// places the 2 rooms where keys can be grabbed by captives
	/// </summary>
	void KeyRooms();


	class CorridorMaker
	{
	private:
		int scale = 2;
		int spacing = 6; //unaffected by scale

		int minExtras = 7;
		int maxExtras = 12;

		CorridorNode* nodes = nullptr;
		bool* converted = nullptr;
		Tile* map;
		intV2 size;
		intV2 scaledSize;

		int Index(intV2 node) { return node.x + node.y * scaledSize.x; }

		void GenerateConnections();

		void ConvertNodeToTiles(intV2 node, bool first);

		void AddExtraConnections();

	public:

		CorridorMaker(intV2 size);
		
		Tile* CreateCorridorMap();
	};


	/// <summary>
	/// generates the corridors to connect rooms
	/// </summary>
	void GenerateCorridors();


	/// <summary>
	/// Adds the rooms ontop of the corridors
	/// </summary>
	void AddRooms();


	void AddWalls();

	/// <summary>
	/// Checks if a cell should be a wall assuming it is not on the borders
	/// </summary>
	bool IsWall(intV2 pos);

//Running Only-----------------------------------------------
	intV2 majorRooms[3];
	
	intV2 leverRoomPos[leverRoomCount];

	intV2 keyRoomPos[keyRoomCount];

//-----------------------------------------------------------
	Tile* map;


public:

	MapMaker(unsigned int seed) { this->seed = seed; }

	intV2 size;


	Tile* MakeMap();

};