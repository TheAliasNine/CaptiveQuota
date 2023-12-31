#pragma once

#include "v2.h"

class SceneObject
{
public:
	v2 position;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(v2 camPos) = 0;
};