#pragma once

#include "Collider.h"
#include "UIElement.h"

#include "v2.h"


class Color;
class Button : public UIElement
{
	char* text = nullptr;
	Color textColor = Color();

	bool drawArea = false;
	Color drawColor = Color();
	int fontSize = 0;
public:
	Button() {}
	Button(Collider* area, char* text, bool drawArea, Color areaColor, Color textColor, int fontSize);

	void Update() {}
	void Draw();

	void OnFocus() {}
	void OnUnfocus() {}
	void OnClick();
};