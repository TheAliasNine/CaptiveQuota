#include "AStarPathFinder.h"

AStarPathFinder::AStarPathFinder()
{
	p_map = nullptr;

	m_current = nullptr;
	m_closedList = nullptr;
	m_nodes = nullptr;
	m_heuristicModifier = 1;
}
AStarPathFinder::AStarPathFinder(Map* map)
{
	p_map = map;

	m_size = p_map->Size();

	m_nodes = new AStarNode[m_size.x * m_size.y];
	for (int i = 0; i < m_size.x * m_size.y; i++)
	{
		m_nodes[i] = AStarNode();
		int x = i % m_size.x;
		int y = i / m_size.x;
		m_nodes[i].SetVals(intV2{x, y}, (*p_map)[i] != Map::Tile::wall && (*p_map)[i] != Map::Tile::exit);
	}
	m_current = &m_nodes[0];

	m_closedList = new bool[p_map->Size().x * p_map->Size().y];
	for (int i = 0; i < p_map->Size().x * p_map->Size().y; i++)
	{
		m_closedList[i] = false;
	}
	m_heuristicModifier = 1;
}

#pragma region Rule of 5
AStarPathFinder::~AStarPathFinder()
{
	delete[] m_nodes;
	delete[] m_closedList;

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

	m_current = &m_nodes[p_map->Index(other.m_current->Pos())];
	
	m_openList = other.m_openList;
	m_closedList = new bool[p_map->Size().x * p_map->Size().y];
	for (int i = 0; i < p_map->Size().x * p_map->Size().y; i++)
	{
		m_closedList[i] = other.m_closedList[i];
	}
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

	m_current = &m_nodes[p_map->Index(other.m_current->Pos())];

	m_openList = other.m_openList;
	delete[] m_closedList;
	m_closedList = new bool[m_size.x * m_size.y];
	for (int i = 0; i < p_map->Size().x * p_map->Size().y; i++)
	{
		m_closedList[i] = other.m_closedList[i];
	}

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

	m_current = other.m_current;
	m_openList = other.m_openList;
	m_closedList = other.m_closedList;
	other.m_closedList = nullptr;
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

	delete[] m_closedList;
	m_closedList = other.m_closedList;
	other.m_closedList = nullptr;

	m_current = other.m_current;

	return *this;
}
#pragma endregion

AStarPath AStarPathFinder::ConstructPath(AStarNode pathEnd)
{
	return AStarPath(pathEnd);
}

AStarPath AStarPathFinder::FindPathToGoal()
{
	while (!m_openList.empty() && !m_foundGoal)
	{
		ProcessNextNode();
	}
	if (m_foundGoal)
	{
		return ConstructPath(m_nodes[p_map->Index(m_goal)]);
	}
	else
	{
		return AStarPath();
	}
}

void AStarPathFinder::Reset()
{
	m_start = intV2();
	m_goal = intV2();

	m_foundGoal = false;
	m_openList.clear();
	for (int i = 0; i < p_map->Size().x * p_map->Size().y; i++)
	{
		m_closedList[i] = false;
	}
}
void AStarPathFinder::Setup(intV2 start, intV2 goal)
{
	Reset();
	m_start = start;
	m_goal = goal;
	SetNodeVals(m_nodes[start.x + start.y * m_size.x], nullptr);
	m_openList.push_front(&m_nodes[start.x + start.y * m_size.x]);

}
void AStarPathFinder::Setup(intV2 start, intV2 goal, float heuristicModifier)
{
	m_heuristicModifier = heuristicModifier;
	Setup(start, goal);
}

AStarNode& AStarPathFinder::NextNode()
{
	return *m_openList.front();
}
AStarNode& AStarPathFinder::JustProcessedNode()
{
	return *m_current;
}

void AStarPathFinder::ProcessNextNode()
{
	m_current = m_openList.front();

	m_openList.pop_front();

	m_closedList[p_map->Index(m_current->Pos())] = true;
	
	if (m_current->Pos() == m_goal)
		m_foundGoal = true;

	int connectionCount = 0;
	AStarNode** connections = GetConnectedNodes(m_current, connectionCount);

	for (int i = 0; i < connectionCount; i++)
	{
		int newNodeInd = p_map->Index(connections[i]->Pos());

		
		bool exists = m_closedList[newNodeInd];

		if (!exists)
		{
			for (auto iter = m_openList.begin(); iter != m_openList.end(); iter++)
			{
				if (*iter != &m_nodes[newNodeInd]) continue;

				exists = true;
				break;
			}
		}

		if (exists)
		{
			CheckRecalculateNode(*connections[i], *m_current);
			continue;
		}
		
		SetNodeVals(*connections[i], m_current);

		
		if (connections[i]->Traversible())
		{
			int openListCount = m_openList.size();
			for (auto iter = m_openList.begin(); iter != m_openList.end(); iter++)
			{
				if ((*iter)->F() > connections[i]->F())
				{
					m_openList.insert(iter, connections[i]);
					break;
				}
			}
			if (openListCount == m_openList.size())
				m_openList.push_back(connections[i]);
		}
		else
		{
			m_closedList[p_map->Index(connections[i]->Pos())] = true;
		}
		if (connections[i]->Pos() == m_goal)
			m_foundGoal = true;
	}
	delete[] connections;
}

bool AStarPathFinder::FoundGoal()
{
	return m_foundGoal; 
}

int AStarPathFinder::CalcHeuristicDistance(intV2 pointA, intV2 pointB)
{
	return CalcDistance(pointA, pointB) * m_heuristicModifier;
}
int AStarPathFinder::CalcDistance(intV2 pointA, intV2 pointB)
{
	int dx = abs(pointA.x - pointB.x);
	int dy = abs(pointA.y - pointB.y);

	if (dx > dy) return (14 * dy) + 10 * (dx - dy);
	else return (14 * dx) + 10 * (dy - dx);
}

void AStarPathFinder::SetNodeVals(AStarNode& node, AStarNode* previous)
{
	node.SetH(CalcHeuristicDistance(node.Pos(), m_goal));
	node.m_previous = previous;
	if (previous != nullptr)
		node.SetG(previous->m_g + CalcDistance(node.m_pos, previous->m_pos));
}

AStarNode** AStarPathFinder::GetConnectedNodes(AStarNode* node, int& outAmount)
{
	AStarNode** connected = new AStarNode*[8];
	int connections = 0;

	intV2 nodePos = node->Pos();

	bool upTraversible = false;
	bool rightTraversible = false;
	bool leftTraversible = false;
	bool downTraversible = false;

	//top
	if (nodePos.y > 0)
	{
		connected[connections] = &m_nodes[p_map->Index(intV2{ nodePos.x, nodePos.y - 1 })];
		connections++;
		upTraversible = m_nodes[p_map->Index(intV2{ nodePos.x, nodePos.y - 1 })].m_traversible;
	}
	//left
	if (nodePos.x > 0)
	{
		connected[connections] = &m_nodes[p_map->Index(intV2{ nodePos.x - 1, nodePos.y })];
		connections++;
		leftTraversible = m_nodes[p_map->Index(intV2{ nodePos.x - 1, nodePos.y })].m_traversible;
	}
	//right
	if (nodePos.x < p_map->Size().x - 1)
	{
		connected[connections] = &m_nodes[p_map->Index(intV2{ nodePos.x + 1, nodePos.y })];
		connections++;
		rightTraversible = m_nodes[p_map->Index(intV2{ nodePos.x + 1, nodePos.y })].m_traversible;
	}
	//bottom
	if (nodePos.y < p_map->Size().y - 1)
	{
		connected[connections] = &m_nodes[p_map->Index(intV2{ nodePos.x, nodePos.y + 1 })];
		connections++;
		downTraversible = m_nodes[p_map->Index(intV2{ nodePos.x, nodePos.y + 1 })].m_traversible;
	}
	//top left
	if (nodePos.x > 0 && nodePos.y > 0 && upTraversible && leftTraversible)
	{
		connected[connections] = &m_nodes[p_map->Index(intV2{ nodePos.x - 1, nodePos.y - 1 })];
		connections++;
	}
	//top right
	if (nodePos.x < p_map->Size().x - 1 && nodePos.y > 0 && upTraversible && rightTraversible)
	{
		connected[connections] = &m_nodes[p_map->Index(intV2{ nodePos.x + 1, nodePos.y - 1 })];
		connections++;
	}
	//bottom left
	if (nodePos.x > 0 && nodePos.y < p_map->Size().y - 1 && downTraversible && leftTraversible)
	{
		connected[connections] = &m_nodes[p_map->Index(intV2{ nodePos.x - 1, nodePos.y + 1 })];
		connections++;
	}
	//bottom right
	if (nodePos.x < p_map->Size().x - 1 && nodePos.y < p_map->Size().y - 1 && downTraversible && rightTraversible)
	{
		connected[connections] = &m_nodes[p_map->Index(intV2{ nodePos.x + 1, nodePos.y + 1 })];
		connections++;
	}
	outAmount = connections;
	return connected;
}

void AStarPathFinder::CheckRecalculateNode(AStarNode& node, AStarNode& newConnection)
{
	int checkH = CalcHeuristicDistance(node.Pos(), m_goal);
	int checkG = newConnection.m_g + CalcDistance(node.m_pos, newConnection.m_pos);
	if (checkH + checkG < node.F())
		RecalculateNode(node, newConnection);
}

void AStarPathFinder::RecalculateNode(AStarNode& node, AStarNode& previous)
{
	if (previous.Previous() == &node)
	{
		int i = 0;
	}
	
	SetNodeVals(node, &previous);
	int connectionsCount = 0;
	AStarNode** connections = GetConnectedNodes(&node, connectionsCount);

	for (int i = 0; i < connectionsCount; i++)
	{
		if (connections[i]->Previous() == &node)
			RecalculateNode(*connections[i], node);
	}
	delete[] connections;
}