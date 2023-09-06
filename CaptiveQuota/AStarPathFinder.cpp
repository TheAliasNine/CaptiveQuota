#include "AStarPathFinder.h"

AStarPathFinder::AStarPathFinder(Map* map)
{
	p_map = map;

	m_size = p_map->Size();

	m_nodes = new AStarNode[m_size.x * m_size.y];
}

#pragma region Rule of 5
AStarPathFinder::~AStarPathFinder()
{
	delete[] m_nodes;
}
AStarPathFinder::AStarPathFinder(const AStarPathFinder& other)
{
	p_map = other.p_map;
	m_size = other.m_size;
	m_foundGoal = other.m_foundGoal;
	m_start = other.m_start;
	m_goal = other.m_goal;
	m_heuristicModifier = other.m_heuristicModifier;
	
	m_nodes = new AStarNode[m_size.x * m_size.y];
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		m_nodes[i] = other.m_nodes[i];
	}
	
	m_openList = other.m_openList;
	m_closedList = other.m_closedList;
}
AStarPathFinder& AStarPathFinder::operator= (const AStarPathFinder& other)
{
	p_map = other.p_map;
	m_size = other.m_size;
	m_foundGoal = other.m_foundGoal;
	m_start = other.m_start;
	m_goal = other.m_goal;
	m_heuristicModifier = other.m_heuristicModifier;

	delete[] m_nodes;
	m_nodes = new AStarNode[m_size.x * m_size.y];
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		m_nodes[i] = other.m_nodes[i];
	}

	m_openList = other.m_openList;
	m_closedList = other.m_closedList;

	return *this;
}
AStarPathFinder::AStarPathFinder(AStarPathFinder&& other)
{
	p_map = other.p_map;
	m_size = other.m_size;
	m_foundGoal = other.m_foundGoal;
	m_start = other.m_start;
	m_goal = other.m_goal;
	m_heuristicModifier = other.m_heuristicModifier;

	m_nodes = other.m_nodes;
	other.m_nodes = nullptr;

	m_openList = other.m_openList;
	m_closedList = other.m_closedList;
}
AStarPathFinder& AStarPathFinder::operator= (AStarPathFinder&& other)
{
	p_map = other.p_map;
	m_size = other.m_size;
	m_foundGoal = other.m_foundGoal;
	m_start = other.m_start;
	m_goal = other.m_goal;
	m_heuristicModifier = other.m_heuristicModifier;

	delete[] m_nodes;
	m_nodes = other.m_nodes;
	other.m_nodes = nullptr;

	m_openList = other.m_openList;
	m_closedList = other.m_closedList;

	return *this;
}
#pragma endregion

AStarPath AStarPathFinder::ConstructPath(AStarNode pathEnd)
{
	return AStarPath(pathEnd);
}

AStarPath AStarPathFinder::FindPathToGoal()
{



	return ConstructPath(m_nodes[p_map->Index(m_goal)]);
}

void AStarPathFinder::Reset()
{
	m_start = intV2();
	m_goal = intV2();
	m_foundGoal = false;
	m_openList.empty();
	m_closedList.empty();
}
void AStarPathFinder::Setup(intV2 start, intV2 goal)
{
	Reset();
	m_start = start;
	m_goal = goal;

}
void AStarPathFinder::Setup(intV2 start, intV2 goal, float heuristicModifier)
{
	Setup(start, goal);
	m_heuristicModifier = heuristicModifier;
}

AStarNode& AStarPathFinder::NextNode()
{
	return *m_openList.front();
}
AStarNode& AStarPathFinder::JustProcessedNode()
{
	return *m_closedList.back();
}

void AStarPathFinder::ProcessNextNode()
{

}

bool AStarPathFinder::FoundGoal()
{
	return m_foundGoal;
}