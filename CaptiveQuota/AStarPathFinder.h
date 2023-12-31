#pragma once

#include "AStarNode.h"
#include "AStarPath.h"

#include "Map.h"

#include <list>
#include <vector>

class AStarPathFinder
{
	Map* p_map;
	
	intV2 m_size;
	bool m_foundGoal;
	
	intV2 m_start;

	intV2 m_goal;
	float m_heuristicModifier; //set to negative to wander further from the goal, as in a runaway attempt


	AStarNode* m_nodes;

	AStarNode* m_current;
	
	//use a list due to insertion being faster than on a vector
	std::list<AStarNode*> m_openList;
	
	bool* m_closedList;
	

	int CalcHeuristicDistance(intV2 pointA, intV2 pointB);
	int CalcDistance(intV2 pointA, intV2 pointB);
	void SetNodeVals(AStarNode& node, AStarNode* previous);

	AStarNode** GetConnectedNodes(AStarNode* node, int& outAmount);

	void CheckRecalculateNode(AStarNode& node, AStarNode& newConnection);
	void RecalculateNode(AStarNode& node, AStarNode& previous);

public:
	AStarPathFinder();
	AStarPathFinder(Map* map);

	~AStarPathFinder();
	AStarPathFinder(const AStarPathFinder& other);
	AStarPathFinder& operator= (const AStarPathFinder& other);
	AStarPathFinder(AStarPathFinder&& other);
	AStarPathFinder& operator= (AStarPathFinder&& other);

	AStarPath FindPathToGoal();
	AStarPath ConstructPath(AStarNode pathEnd);


	void Reset();
	void Setup(intV2 start, intV2 goal);
	void Setup(intV2 start, intV2 goal, float heuristicModifier);

	AStarNode& NextNode();
	AStarNode& JustProcessedNode();

	void ProcessNextNode();

	bool FoundGoal();
};