#pragma once

class SceneObject
{
public:
	v2 position;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
};