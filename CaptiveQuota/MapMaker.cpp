#include "Map.h"


#include "CorridorNode.h"
#include "v2.h"

#include <list>
#include <random>

#ifdef _DEBUG
#include <chrono>
#include <iostream>
#endif // _DEBUG

Map* Map::MapMaker::MakeMap()
{
#ifdef  _DEBUG
	auto before = std::chrono::steady_clock::now();
#endif //  _DEBUG


	std::srand(seed);
	MajorRooms();

	LeverRooms();
	KeyRooms();
	GenerateCorridors();
	AddRooms();
	AddWalls();

#ifdef _DEBUG
	std::chrono::steady_clock::time_point after = std::chrono::steady_clock::now();
	std::chrono::duration<double> processTime = after - before;
	std::cout << "Map Generation took " << processTime.count() << " seconds." << std::endl;

#endif //  _DEBUG

	return map;
}

void Map::MapMaker::MajorRooms()
{
	//Calculate the starting location for the 3 major locations
	v2 a = v2();
	
	float distance = (std::rand() % (majorMaxDistance - majorMinDistance)) + majorMinDistance;	
	float rotation = float(std::rand() % 314) / 100;
	v2 b = v2(distance, 0).RotateVector(rotation);

	v2 c = b.RotateVector(std::_Pi / 3);

	float lowestX = a.x;
	if (b.x < lowestX)
		lowestX = b.x;
	if (c.x < lowestX)
		lowestX = c.x;

	float lowestY = a.y;
	if (b.y < lowestY)
		lowestY = b.y;
	if (c.y < lowestY)
		lowestY = c.y;

	lowestX -= majorRoomSize + 1;
	lowestY -= majorRoomSize + 1;


	a.x -= lowestX;
	a.y -= lowestY;
	b.x -= lowestX;
	b.y -= lowestY;
	c.x -= lowestX;
	c.y -= lowestY;
	
	majorRooms[0].x = round(a.x);
	majorRooms[0].y = round(a.y);

	majorRooms[1].x = round(b.x);
	majorRooms[1].y = round(b.y);

	majorRooms[2].x = round(c.x);
	majorRooms[2].y = round(c.y);


	//figure out the max size
	map->m_size.x = majorRooms[0].x;
	map->m_size.y = majorRooms[0].y;
	for (int i = 1; i < majorRoomCount; i++)
	{
		if (map->m_size.x < majorRooms[i].x)
			map->m_size.x = majorRooms[i].x;
		if (map->m_size.y < majorRooms[i].y)
			map->m_size.y = majorRooms[i].y;
	}
	//accomodate for room size
	map->m_size.x += majorRoomSize + 1;
 	map->m_size.y += majorRoomSize + 1;
	//add one to include both 0 and highest val
	map->m_size.x++;
	map->m_size.y++;
}


void Map::MapMaker::LeverRooms()
{
	//assign random positions not in the major rooms
	for (int i = 0; i < leverRoomCount; i++)
	{
		map->m_levers[i].x = (rand() % (map->m_size.x - 2 - leverRoomSize * 2)) + leverRoomSize + 1;
		map->m_levers[i].y = (rand() % (map->m_size.y - 2 - leverRoomSize * 2)) + leverRoomSize + 1;

		bool overlapped = false;
		for (int j = 0; j < majorRoomCount; j++)
		{
			if (std::abs(map->m_levers[i].x - majorRooms[j].x) <= majorRoomSize + leverRoomSize &&
				std::abs(map->m_levers[i].y - majorRooms[j].y) <= majorRoomSize + leverRoomSize)
			{
				overlapped = true;
				break;
			}
		}

		if (overlapped)
		{
			i--;
			continue;
		}


		//check overlap with other lever rooms
		for (int j = 0; j < i; j++)
		{
			if (std::abs(map->m_levers[i].x - map->m_levers[j].x) <= minLeverRoomDist && 
				std::abs(map->m_levers[i].y - map->m_levers[j].y) <= minLeverRoomDist)
			{
				overlapped = true;
				break;
			}
			
		}

		if (overlapped) i--;
	}
}

void Map::MapMaker::KeyRooms()
{
	//assign random positions
	//assign random positions not in the major rooms
	for (int i = 0; i < keyRoomCount; i++)
	{
		intV2 position = map->m_keyMolds[i];
		position.x = (rand() % (map->m_size.x - 2 - keyRoomSize * 2)) + keyRoomSize + 1;
		position.y = (rand() % (map->m_size.y - 2 - keyRoomSize * 2)) + keyRoomSize + 1;

		bool overlapped = false;
		for (int j = 0; j < majorRoomCount; j++)
		{
			if (std::abs(position.x - majorRooms[j].x) <= keyRoomSize + majorRoomSize &&
				std::abs(position.y - majorRooms[j].y) <= keyRoomSize + majorRoomSize)
			{
				overlapped = true;
				break;
			}
		}

		if (overlapped)
		{
			i--;
			continue;
		}

		for (int j = 0; j < leverRoomCount; j++)
		{
			if (std::abs(position.x - map->m_levers[j].x) <= keyRoomSize + leverRoomSize && 
				std::abs(position.y - map->m_levers[j].y) <= keyRoomSize + leverRoomSize)
			{
				overlapped = true;
				break;
			}
		}

		if (overlapped)
		{
			i--;
			continue;
		}

		//check overlap with other key rooms
		for (int j = 0; j < i; j++)
		{
			if (std::abs(position.x - map->m_keyMolds[j].x) <= minKeyRoomDist &&
				std::abs(position.y - map->m_keyMolds[j].y) <= minKeyRoomDist)
			{
				overlapped = true;
				break;
			}
		}

		if (overlapped) i--;
		else map->m_keyMolds[i] = position;
	}
}

void Map::MapMaker::GenerateCorridors()
{
	CorridorMaker corridorMaker = CorridorMaker(map, map->m_size);
	corridorMaker.CreateCorridorMap();
}

void Map::MapMaker::AddRooms()
{
	int totalMajorRoomSize = 2 * majorRoomSize + 1;
	for (int i = 0; i < totalMajorRoomSize * totalMajorRoomSize; i++)
	{
		int x = i % totalMajorRoomSize;
		int y = i / totalMajorRoomSize;

		x -= majorRoomSize;
		y -= majorRoomSize;
		
		map->m_tiles[x + majorRooms[0].x + (y + majorRooms[0].y) * map->m_size.x] = Map::Tile::exitRoom;

		map->m_tiles[x + majorRooms[1].x + (y + majorRooms[1].y) * map->m_size.x] = Map::Tile::vault;

		map->m_tiles[x + majorRooms[2].x + (y + majorRooms[2].y) * map->m_size.x] = Map::Tile::cell; // the cell entirely is covered with different flooring
	}

	//Create Exit -------------------------------------------------------------------------
	map->m_playerSpawn = majorRooms[0];
	intV2 direction = intV2();
	intV2 centre = intV2{ (int)round((map->m_size.x - 1) / 2), (int)round((map->m_size.y - 1) / 2) };
	if (abs(centre.x - majorRooms[0].x) > abs(centre.y - majorRooms[0].y))
	{
		direction.x = centre.x - majorRooms[0].x > 0 ? -1 : 1;
	}
	else
	{
		direction.y = centre.y - majorRooms[0].y > 0 ? -1 : 1;
	}
	map->m_exit = intV2{ majorRooms[0].x + (direction.x * (majorRoomSize + 1)), majorRooms->y + (direction.y * (majorRoomSize + 1)) };
	map->m_tiles[map->m_exit.x + map->m_exit.y * map->m_size.x] = Map::Tile::exit;
	//-------------------------------------------------------------------------------------

	int totalKeyRoomSize = 2 * keyRoomSize + 1;
	for (int room = 0; room < keyRoomCount; room++)
	{
		for (int i = 0; i < totalKeyRoomSize * totalKeyRoomSize; i++)
		{
			int x = i % totalKeyRoomSize;
			int y = i / totalKeyRoomSize;

			x -= keyRoomSize;
			y -= keyRoomSize;

			
			map->m_tiles[x + map->m_keyMolds[room].x + (y + map->m_keyMolds[room].y) * map->m_size.x] = Map::Tile::path;
		}
	}


	int totalLeverRoomSize = 2 * leverRoomSize + 1;
	for (int room = 0; room < leverRoomCount; room++)
	{
		for (int i = 0; i < totalLeverRoomSize * totalLeverRoomSize; i++)
		{
			int x = i % totalLeverRoomSize;
			int y = i / totalLeverRoomSize;

			x -= leverRoomSize;
			y -= leverRoomSize;

			map->m_tiles[x + map->m_levers[room].x + (y + map->m_levers[room].y) * map->m_size.x] = Map::Tile::path;
		}
	}
	
}

void Map::MapMaker::AddWalls()
{
	//for every cell except the borders - saves checks for every cell
	for (int y = 1; y < map->m_size.y - 1; y++)
	{
		for (int x = 1; x < map->m_size.x - 1; x++)
		{
			if (!IsWall(intV2{ x, y })) continue;
			map->m_tiles[x + y * map->m_size.x] = Map::Tile::wall;
		}
	}


	
	//entire side excluding corners
	for (int x = 1; x < map->m_size.x - 1; x++)
	{
		if (map->m_tiles[x + map->m_size.x] != Map::Tile::null && map->m_tiles[x + map->m_size.x] != Map::Tile::wall)
		{
			if (map->m_tiles[x] == Map::Tile::null)
				map->m_tiles[x] = Map::Tile::wall;
			if (map->m_tiles[x + 1] == Map::Tile::null)
				map->m_tiles[x + 1] = Map::Tile::wall;
			if (map->m_tiles[x - 1] == Map::Tile::null)
				map->m_tiles[x - 1] = Map::Tile::wall;

		}
		if (map->m_tiles[x + map->m_size.x * (map->m_size.y - 2)] != Map::Tile::null && map->m_tiles[x + map->m_size.x * (map->m_size.y - 2)] != Map::Tile::wall)
		{
			if (map->m_tiles[x + map->m_size.x * (map->m_size.y - 1)] == Map::Tile::null)
				map->m_tiles[x + map->m_size.x * (map->m_size.y - 1)] = Map::Tile::wall;
			if (map->m_tiles[x + map->m_size.x * (map->m_size.y - 1) + 1] == Map::Tile::null)
				map->m_tiles[x + map->m_size.x * (map->m_size.y - 1) + 1] = Map::Tile::wall;
			if (map->m_tiles[x + map->m_size.x * (map->m_size.y - 1) - 1] == Map::Tile::null)
				map->m_tiles[x + map->m_size.x * (map->m_size.y - 1) - 1] = Map::Tile::wall;
		}
	}
	for (int y = 1; y < map->m_size.y - 1; y++)
	{
		if (map->m_tiles[y * map->m_size.x + 1] != Map::Tile::null && map->m_tiles[y * map->m_size.x + 1] != Map::Tile::wall)
		{
			if (map->m_tiles[y * map->m_size.x] == Map::Tile::null)
				map->m_tiles[y * map->m_size.x] = Map::Tile::wall;
			if (map->m_tiles[y * map->m_size.x + map->m_size.x] == Map::Tile::null)
				map->m_tiles[y * map->m_size.x + map->m_size.x] = Map::Tile::wall;
			if (map->m_tiles[y * map->m_size.x - map->m_size.x] == Map::Tile::null)
				map->m_tiles[y * map->m_size.x - map->m_size.x] = Map::Tile::wall;

		}
		if (map->m_tiles[y * map->m_size.x + (map->m_size.x - 1) - 1] != Map::Tile::null && map->m_tiles[y * map->m_size.x + (map->m_size.x - 1) - 1] != Map::Tile::wall)
		{
			if (map->m_tiles[y * map->m_size.x + (map->m_size.x - 1)] == Map::Tile::null)
				map->m_tiles[y * map->m_size.x + (map->m_size.x - 1)] = Map::Tile::wall;
			if (map->m_tiles[(y + 1) * map->m_size.x + (map->m_size.x - 1)] == Map::Tile::null)
				map->m_tiles[(y + 1) * map->m_size.x + (map->m_size.x - 1)] = Map::Tile::wall;
			if (map->m_tiles[(y - 1) * map->m_size.x + (map->m_size.x - 1)] == Map::Tile::null)
				map->m_tiles[(y - 1) * map->m_size.x + (map->m_size.x - 1)] = Map::Tile::wall;
		}
	}

}

bool Map::MapMaker::IsWall(intV2 pos)
{
	if (map->m_tiles[pos.x + pos.y * map->m_size.x] != Map::Tile::null) return false;

	for (int i = 0; i < 8; i++)
	{
		int index = pos.x + pos.y * map->m_size.x;
		switch (i)
		{
		case 0:
			index += -1 + -map->m_size.x;
			break;
		case 1:
			index -= map->m_size.x;
			break;
		case 2:
			index += 1 - map->m_size.x;
			break;
		case 3:
			index--;
			break;
		case 4:
			index++;
			break;
		case 5:
			index += -1 + map->m_size.x;
			break;
		case 6:
			index += map->m_size.x;
			break;
		case 7:
			index += 1 + map->m_size.x;
			break;
		}
		if (map->m_tiles[index] != Map::Tile::null && map->m_tiles[index] != Map::Tile::wall) return true;
	}
	return false;
}


Map::MapMaker::CorridorMaker::CorridorMaker(Map* map, intV2 size)
{
	scaledSize = intV2{ (size.x - 1 - (spacing / 2) - scale) / (spacing + scale) + 1 , (size.y - 1 - (spacing / 2) - scale) / (spacing + scale) + 1 };
	map->m_size = size;
	map->m_tiles = new Map::Tile[size.x * size.y];
	for (int i = 0; i < size.x * size.y; i++)
	{
		map->m_tiles[i] = Map::Tile::null;
	}
	this->map = map;
}

void Map::MapMaker::CorridorMaker::CreateCorridorMap()
{
	nodes = new CorridorNode[scaledSize.x * scaledSize.y];

	for (int i = 0; i < scaledSize.x * scaledSize.y; i++)
	{
		nodes[i] = CorridorNode(intV2{ i % scaledSize.x, i / scaledSize.x }, scaledSize);
	}
	GenerateConnections();
	AddExtraConnections();
	ConvertNodeToTiles(intV2(), true);
	delete[] nodes;
	nodes = nullptr;
}

void Map::MapMaker::CorridorMaker::GenerateConnections()
{
	std::list<CorridorNode*> openList;
	std::list<CorridorNode*> closedList;

	closedList.push_front(&nodes[0]);

	do
	{
		CorridorNode* current = closedList.back();
		//neighbour order
		int* nbOrder = new int[current->noNeighbours];
		for (int i = 0; i < current->noNeighbours; i++)
		{
			nbOrder[i] = -1;
		}

		for (int i = 0; i < current->noNeighbours; i++)
		{
			//select 1 of the random neighbours for this iteration (no repeats)
			while (nbOrder[i] == -1) //while haven't found index for this iteration
			{
				int index = std::rand() % current->noNeighbours; //get random one

				bool exists = false;
				for (int j = 0; j < current->noNeighbours; j++) //see if random one already exists
				{
					if (index != nbOrder[j]) continue;
					exists = true;
					break;
				}
				if (!exists) nbOrder[i] = index;
			}


			//make sure neighbour isn't already connected (ie. on the closed list)
			bool found = false;
			for (auto iter = closedList.begin(); iter != closedList.end(); iter++)
			{
				if ((*iter)->node != current->neighbours[nbOrder[i]]) continue;
				found = true;
				break;
			}
			if (found) continue;

			//if on open list erase so it can than be added to front again
			for (auto iter = openList.begin(); iter != openList.end(); iter++)
			{
				if ((*iter)->node != current->neighbours[nbOrder[i]]) continue;
				openList.erase(iter);
				break;
			}
			openList.push_front(&nodes[Index(current->neighbours[nbOrder[i]])]);
		}
		delete[] nbOrder;

		//make connection to the first in openList
		//can't assume that the first in openList is directly connected to the current so have to backtrack until there is a connection to it

		bool connectionMade = false;
		while (!connectionMade)
		{
			for (int i = 0; i < current->noNeighbours; i++)
			{
				if (current->neighbours[i] != openList.front()->node) continue;
				current->AddConnection(openList.front()->node);
				openList.front()->AddConnection(current->node);

				closedList.push_back(openList.front());
				openList.pop_front();

				connectionMade = true;
				break;
			}
			//connections[0] should always be the node previous to it as it should be the first connection made
			current = &nodes[Index(current->connections[0])];
		}

	} while (!openList.empty());

}


void Map::MapMaker::CorridorMaker::AddExtraConnections()
{
	int makeConnectionsNo = std::rand() % (maxExtras - minExtras) + minExtras;

	for (int i = 0; i < makeConnectionsNo; i++)
	{
		//pick a random node
		int x = std::rand() % scaledSize.x;
		int y = std::rand() % scaledSize.y;

		//try to add a connection to a random neighbour
		int index = Index(intV2{ x, y });
		if (nodes[index].noConnections == nodes[index].noNeighbours)
		{
			i--;
			continue;
		}
		
		if (i == 6)
		{
			int assd = 0;
		}
		int openNo = 0;
		intV2 openNeighbours[4] = {};
		for (int j = 0; j < nodes[index].noNeighbours; j++)
		{
			bool found = false;
			for (int k = 0; k < nodes[index].noConnections; k++)
			{
				if (nodes[index].neighbours[j] != nodes[index].connections[k]) continue;
				found = true;
				break;
			}
			if (found) continue;
			openNeighbours[openNo] = nodes[index].neighbours[j];
			openNo++;
		}

		int chosenNeighbour = std::rand() % openNo;

		nodes[Index(openNeighbours[chosenNeighbour])].AddConnection(intV2{x, y});
		nodes[index].AddConnection(openNeighbours[chosenNeighbour]);
	}

}


void Map::MapMaker::CorridorMaker::ConvertNodeToTiles(intV2 node, bool first)
{
	if (first)
	{
		converted = new bool[scaledSize.x * scaledSize.y];
		for (int i = 0; i < scaledSize.x * scaledSize.y; i++)
		{
			converted[i] = false;
		}
	}

	converted[Index(node)] = true;

	int offsetFromBorder = (spacing / 2) * (map->m_size.x + 1);
	int nodePosition = (node.x + node.y * map->m_size.x) * (spacing + scale);

	//create the node
	for (int y = 0; y < scale; y++)
	{
		for (int x = 0; x < scale; x++)
		{
			//                                            filling out scale
			int index = offsetFromBorder + nodePosition + (x + y * map->m_size.x);
			if (index >= map->m_size.x * map->m_size.y)
			{
				int asda = 0;
			}
			map->m_tiles[index] = Map::Tile::path;
		}
	}

	int startNode = first ? 0 : 1;
	for (int i = startNode; i < nodes[Index(node)].noConnections; i++)
	{
		//draw connection
		intV2 direction = nodes[Index(node)].connections[i] - node;

		int offsetFromNode = 0;
		if (direction.x > 0)
			offsetFromNode = scale;
		if (direction.x < 0)
			offsetFromNode = -1;
		else if (direction.y > 0)
			offsetFromNode = map->m_size.x * scale;
		else if (direction.y < 0)
			offsetFromNode = -map->m_size.x;


		for (int j = 0; j < spacing; j++)
		{
			int spacingFill = (direction.x + direction.y * map->m_size.x) * j;
			for (int k = 0; k < scale; k++)
			{
				//                                                                                                fill scale
				int index = offsetFromBorder + nodePosition + spacingFill + offsetFromNode +     k * abs(direction.y) + k * abs(direction.x) * map->m_size.x;
				
				map->m_tiles[index] = Map::Tile::path;
			}
		}

		//call to continue conversion
		if (!converted[Index(nodes[Index(node)].connections[i])])
			ConvertNodeToTiles(nodes[Index(node)].connections[i], false);
	}
	if (first)
	{
		delete[] converted;
		converted = nullptr;
	}
}