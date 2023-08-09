#pragma once

#include "CorridorNode.h"
#include "intV2.h"

struct Map
{
public:
	Map();
	~Map();

	Map(const Map& other);
	Map& operator= (const Map& other);
	Map(Map&& other);
	Map& operator= (Map&& other);

	void CreateMap(unsigned int seed);

	enum class Tile
	{
		null,
		wall,
		path,
		cell,
		exit,
		vault,
		key,
		lever,
	};


	Tile operator[] (int index);
	Tile operator[] (intV2 pos);

	intV2 PlayerSpawn() { return m_playerSpawn; }
	intV2 Exit() { return m_exit; }
	intV2 Size() { return m_size; }


private:

#pragma region MapMaker
	friend class MapMaker;
	friend class CorridorMaker;
	class MapMaker
	{
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
			Map* map = nullptr;
			intV2 scaledSize;

			int Index(intV2 node) { return node.x + node.y * scaledSize.x; }

			void GenerateConnections();

			void ConvertNodeToTiles(intV2 node, bool first);

			void AddExtraConnections();

		public:

			CorridorMaker(Map* map, intV2 size);

			void CreateCorridorMap();
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
		Map* map;


	public:

		MapMaker(Map* map, unsigned int seed)
		{
			this->map = map;
			this->seed = seed;
		}

		Map* MakeMap();

	};
#pragma endregion


	unsigned int m_seed;

	intV2 m_size;
	Tile* m_tiles;

	intV2 m_exit;
	intV2 m_playerSpawn;
	intV2 m_cellMax;
	intV2 m_cellMin;
	intV2 m_portal;
};