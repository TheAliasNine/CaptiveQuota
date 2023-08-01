#pragma once

#include "UIElement.h"

#include "Collider.h"

class UIInteractable : public UIElement
{
protected:
	Collider* collider;
public:

	bool CheckInteraction(v2 position) { return collider->CheckCollision(position, (CollisionInfo*)nullptr); }
	//Click on interaction
	virtual void OnClick() = 0;

	virtual void OnFocus() = 0;
	virtual void OnUnfocus() = 0;
};