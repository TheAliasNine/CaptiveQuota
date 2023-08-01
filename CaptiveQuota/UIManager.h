#pragma once

#include "UIElement.h"

#include <list>

class UIManager
{
	std::list<UIElement*> elements;
	UIElement* focusedInteractable = nullptr;

public:
	UIManager();
	~UIManager();

	void Update();

	void Draw();

	void AddElement(UIElement* element);
	void RemoveElement(UIElement* element);
};