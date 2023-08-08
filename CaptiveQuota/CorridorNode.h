#pragma once

#include "intV2.h"


#ifdef _DEBUG

#include <iostream>

#endif

struct CorridorNode
{

	intV2 node;

	//all the neigbours
	int noNeighbours;
	intV2 neighbours[4] = {};

	//all the connections
	int noConnections;
	intV2 connections[4] = {};

	CorridorNode()
	{
		node = intV2();
		noNeighbours = 0;
		noConnections = 0;

	}

	CorridorNode(intV2 node, intV2 size)
	{
		this->node = node;
		noNeighbours = 0;
		noConnections = 0;
		
		if (node.x - 1 >= 0)
		{
			neighbours[noNeighbours] = intV2{ node.x - 1, node.y };
			noNeighbours++;
		}
		if (node.y - 1 >= 0)
		{
			neighbours[noNeighbours] = intV2{ node.x, node.y - 1 };
			noNeighbours++;
		}
		if (node.x + 1 < size.x)
		{
			neighbours[noNeighbours] = intV2{ node.x + 1, node.y };
			noNeighbours++;
		}
		if (node.y + 1 < size.y)
		{
			neighbours[noNeighbours] = intV2{ node.x, node.y + 1 };
			noNeighbours++;
		}
	}


	void AddConnection(intV2 neighbour)
	{
#ifdef _DEBUG

		bool inConnections = false;
		for (int i = 0; i < noNeighbours; i++)
		{
			if (neighbours[i] == neighbour)
				inConnections = true;
		}
		if (!inConnections)
			std::cout << "Map Generation trying to add a connection to non neighbour cell." << std::endl;

		for (int i = 0; i < noConnections; i++)
		{
			if (connections[i] != neighbour) continue;
			std::cout << "Map Generation trying to add an already existing connection";
		}
#endif

		connections[noConnections] = neighbour;
		noConnections++;
	}

};