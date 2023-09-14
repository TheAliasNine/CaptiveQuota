#include "AStarPath.h"

AStarPath::AStarPath()
{
	m_pathLength = 0;
	m_path = nullptr;
	m_current = 0;
}

AStarPath::AStarPath(AStarNode endNode)
{
	m_current = 0;
	int pathSize = 1;
	for (AStarNode node = endNode; node.Previous() != nullptr; node = *node.Previous())
	{
		pathSize++;
	}
	m_pathLength = pathSize;
	
	m_path = new AStarNode[m_pathLength];
	int i = m_pathLength - 1;
	for (AStarNode* node = &endNode; node != nullptr; node = node->Previous())
	{
		m_path[i] = *node;
		i--;
	}
}


#pragma region Rule of 5
AStarPath::AStarPath(const AStarPath& other)
{
	m_current = other.m_current;
	m_pathLength = other.m_pathLength;

	m_path = new AStarNode[m_pathLength];
	for (int i = 0; i < m_pathLength; i++)
	{
		m_path[i] = other.m_path[i];
	}
}
AStarPath& AStarPath::operator= (const AStarPath& other)
{
	m_current = other.m_current;
	m_pathLength = other.m_pathLength;

	m_path = new AStarNode[m_pathLength];
	for (int i = 0; i < m_pathLength; i++)
	{
		m_path[i] = other.m_path[i];
	}

	return *this;
}
AStarPath::AStarPath(AStarPath&& other)
{
	m_current = other.m_current;
	m_pathLength = other.m_pathLength;

	m_path = other.m_path;
	other.m_path = nullptr;
}
AStarPath& AStarPath::operator= (AStarPath&& other)
{
	m_current = other.m_current;
	m_pathLength = other.m_pathLength;

	m_path = other.m_path;
	other.m_path = nullptr;

	return *this;
}
#pragma endregion


AStarNode AStarPath::Current()
{
	return m_path[m_current];
}

void AStarPath::Progress()
{
	m_current++;
	if (m_current >= m_pathLength)
		m_current = m_pathLength - 1;
}