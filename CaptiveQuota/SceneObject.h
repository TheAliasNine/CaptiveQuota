#pragma once

#include "v2.h"

class SceneObject
{
	v2 position;

public:
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
};