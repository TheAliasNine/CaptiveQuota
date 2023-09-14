#pragma once
#include "Collider.h"
#include "SceneObject.h"

class HitBoxObject : public SceneObject
{
public:
	~HitBoxObject()
	{
		delete m_hitbox;
	}
protected:
	Collider* m_hitbox = nullptr;
public:
	Collider* Hitbox() { return m_hitbox; }
};