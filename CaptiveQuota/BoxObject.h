#pragma once
#include "AABB.h"
#include "SceneObject.h"

class BoxObject : public SceneObject
{
protected:
	AABB m_hitbox;
public:
	AABB Hitbox() { return m_hitbox; }
};