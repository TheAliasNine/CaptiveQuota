#include "Button.h"

#include "raylib.h"

Button::Button(Collider* area, char* text, bool drawArea, Color areaColor, Color textColor, int fontSize)
{
	collider = area;
	this->text = text;
	this->drawArea = drawArea;
	this->drawColor = drawColor;
	this->textColor = textColor;
	this->fontSize = fontSize;
}

void Button::Draw()
{
	if (drawArea)
	{
		collider->Draw(drawColor);
	}
	if (text != nullptr)
	{
		int size = MeasureText(text, fontSize);
		DrawText(text, position.x - size / 2, position.y - fontSize / 2);
	}
}