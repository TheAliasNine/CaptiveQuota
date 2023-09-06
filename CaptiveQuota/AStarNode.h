#pragma once

#include "intV2.h"

class AStarNode
{
	intV2 m_pos;
	bool m_traversible;

	AStarNode* m_previous;
	int m_h; //heuristic
	int m_g; //cost
	int m_f; //pathfinding value

public:
	AStarNode* Previous() { return m_previous; }
};