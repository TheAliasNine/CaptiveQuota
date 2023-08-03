#pragma once

#include "intV2.h"
struct CorridorNode
{

	intV2 node;

	//all the neigbours
	int noNeighbours;
	intV2 neighbours[4];

	//all the connections
	int noConnections;
	intV2 connections[4];
};