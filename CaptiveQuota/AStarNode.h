#pragma once

#include "intV2.h"

struct AStarNode
{
	intV2 m_pos;
	bool m_traversible;

	int m_h; //heuristic
	int m_g; //cost
	int m_f; //pathfinding value

public:

	int H() { return m_h; }
	void SetH(int h)
	{
		m_h = h;
		m_f = m_g + m_h;
	}
	int G() { return m_g; }
	void SetG(int g)
	{
		m_g = g;
		m_f = m_g + m_h;
	}

	int F() { return m_f; }


	AStarNode* m_previous;

	intV2 Pos() { return m_pos; }
	AStarNode* Previous() { return m_previous; }
};