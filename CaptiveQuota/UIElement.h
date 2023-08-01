#pragma once

#include "Collider.h"

#include "v2.h"

class UIElement
{
protected:
	v2 position;
	Collider* collider;

public:

	~UIElement() { delete collider; }

	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void OnFocus() = 0;
	virtual void OnUnfocus() = 0;
	virtual void OnClick() = 0;

	bool CheckInteraction(v2 point) { return collider->CheckCollision(point, (CollisionInfo*) nullptr); }
};