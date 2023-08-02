#include "MapMaker.h"

#include "v2.h"


#include <random>

MapMaker::Tile* MapMaker::MakeMap()
{
	std::srand(seed);
	MajorRooms();
	LeverRooms();
	KeyRooms();
	GenerateCorridors();
	AddRooms();

	return map;
}

void MapMaker::MajorRooms()
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
	size.x = majorRooms[0].x;
	size.y = majorRooms[0].y;
	for (int i = 1; i < majorRoomCount; i++)
	{
		if (size.x < majorRooms[i].x)
			size.x = majorRooms[i].x;
		if (size.y < majorRooms[i].y)
			size.y = majorRooms[i].y;
	}
	//accomodate for room size
	size.x += majorRoomSize + 1;
 	size.y += majorRoomSize + 1;
	//add one to include both 0 and highest val
	size.x++;
	size.y++;
}


void MapMaker::LeverRooms()
{
	//assign random positions not in the major rooms
	for (int i = 0; i < leverRoomCount; i++)
	{
		leverRoomPos[i].x = (rand() % (size.x - leverRoomSize * 2)) + leverRoomSize;
		leverRoomPos[i].y = (rand() % (size.y - leverRoomSize * 2)) + leverRoomSize;

		bool overlapped = false;
		for (int j = 0; j < majorRoomCount; j++)
		{
			if (std::abs(leverRoomPos[i].x - majorRooms[j].x) <= majorRoomSize + leverRoomSize)
				overlapped = true;
			else if (std::abs(leverRoomPos[i].x - majorRooms[j].x) <= majorRoomSize + leverRoomSize)
				overlapped = true;
			if (overlapped) break;
		}

		if (overlapped)
		{
			i--;
			continue;
		}


		//check overlap with other lever rooms
		for (int j = 0; j < i; j++)
		{
			if (std::abs(leverRoomPos[i].x - leverRoomPos[j].x) <= minLeverRoomDist)
			{
				overlapped = true;
				break;
			}
			if (std::abs(leverRoomPos[i].x - leverRoomPos[j].x) <= minLeverRoomDist)
			{
				overlapped = true;
				break;
			}
		}

		if (overlapped) i--;
	}
}

void MapMaker::KeyRooms()
{
	//assign random positions
	//assign random positions not in the major rooms
	for (int i = 0; i < keyRoomCount; i++)
	{
		keyRoomPos[i].x = (rand() % (size.x - keyRoomSize * 2)) + keyRoomSize;
		keyRoomPos[i].y = (rand() % (size.y - keyRoomSize * 2)) + keyRoomSize;

		bool overlapped = false;
		for (int j = 0; j < majorRoomCount; j++)
		{
			if (std::abs(keyRoomPos[i].x - majorRooms[j].x) <= keyRoomSize + majorRoomSize)
				overlapped = true;
			else if (std::abs(keyRoomPos[i].x - majorRooms[j].x) <= keyRoomSize + majorRoomSize)
				overlapped = true;
			if (overlapped) break;
		}

		if (overlapped)
		{
			i--;
			continue;
		}

		for (int j = 0; j < leverRoomCount; j++)
		{
			if (std::abs(keyRoomPos[i].x - leverRoomPos[j].x) <= keyRoomSize + leverRoomSize)
				overlapped = true;
			else if (std::abs(keyRoomPos[i].x - leverRoomPos[j].x) <= keyRoomSize + leverRoomSize)
				overlapped = true;
			if (overlapped) break;
		}

		if (overlapped)
		{
			i--;
			continue;
		}

		//check overlap with other key rooms
		for (int j = 0; j < i; j++)
		{
			if (std::abs(keyRoomPos[i].x - keyRoomPos[j].x) <= keyRoomSize + keyRoomSize)
			{
				overlapped = true;
				break;
			}
			if (std::abs(keyRoomPos[i].x - keyRoomPos[j].x) <= keyRoomSize + keyRoomSize)
			{
				overlapped = true;
				break;
			}
		}

		if (overlapped) i--;
	}
}

void MapMaker::GenerateCorridors()
{
	

}

void MapMaker::AddRooms()
{


}