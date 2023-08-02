#pragma once

#include "intV2.h"
class MapMaker
{
	enum class Tile
	{
		blank,
		path,
		wall,
	};

//Map Making Constants---------------------------------------
	static const int majorRoomCount = 3;
	static const int leverRoomCount = 3;
	static const int keyRoomCount = 2;

	//the size around the centre e.g. 1 = 3x3, 2 = 5x5  (1 + 2x)
	static const int majorRoomSize = 4;
	static const int leverRoomSize = 2;
	static const int keyRoomSize = 2;

	static const int majorMaxDistance = 60;
	static const int majorMinDistance = 40;
	static const int minLeverRoomDist = 15;

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

	/// <summary>
	/// generates the corridors to connect rooms
	/// </summary>
	void GenerateCorridors();

	/// <summary>
	/// Adds the rooms ontop of the corridors
	/// </summary>
	void AddRooms();

//Running Only-----------------------------------------------
	intV2 majorRooms[3];
	
	intV2 leverRoomPos[leverRoomCount];

	intV2 keyRoomPos[keyRoomCount];

//-----------------------------------------------------------
	intV2 size;

	Tile* map;


public:
	Tile* MakeMap();

};