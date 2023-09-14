#pragma once

#include "AStarNode.h"

class AStarPath
{

	int m_pathLength;
	AStarNode* m_path;
	int m_current;

public:
	AStarPath();
	AStarPath(AStarNode endNode);

#pragma region Rule of 5
	AStarPath(const AStarPath& other);
	AStarPath& operator= (const AStarPath& other);
	AStarPath(AStarPath&& other);
	AStarPath& operator= (AStarPath&& other);
#pragma endregion

	AStarNode Index(int i) { return m_path[i]; }
	AStarNode Current();
	int Length() { return m_pathLength; }
	intV2 Goal() { return m_path[m_pathLength - 1].Pos(); }
	void Progress();
};