#include "UIManager.h"

#include "v2.h"
#include "InputManager.h"


UIManager::UIManager()
{

}

UIManager::~UIManager()
{
	for (auto iter = elements.begin(); iter != elements.end(); iter++)
	{
		delete (*iter);
	}
}

void UIManager::Update()
{
	
	if (!InputManager::KeyPressed(InputManager::Confirm)) return;
	//check from front to back so that the ones drawn on to have higher priority
	for (auto iter = elements.rbegin(); iter != elements.rend(); iter++)
	{
		if (!(*iter)->CheckInteraction(v2(GetMousePosition().x, GetMousePosition().y))) continue;
		if (focusedInteractable == (*iter))
		{
			(*iter)->OnUnfocus();
			focusedInteractable = nullptr;
		}
		else
		{
			if (focusedInteractable != nullptr)
				focusedInteractable->OnUnfocus();
			focusedInteractable = (*iter);
			focusedInteractable->OnFocus();
		}
		break;
	}
}


void UIManager::Draw()
{

	//draw standard elements first since it would be preferable to draw elements not needing to be interacted with
	//could make a system to draw in a specific order but meh
	for (auto iter = elements.begin(); iter != elements.end(); iter++)
	{
		(*iter)->Draw();
	}
}


void UIManager::AddElement(UIElement* element)
{
	elements.push_back(element);
}

void UIManager::RemoveElement(UIElement* element)
{
	for (auto iter = elements.begin(); iter != elements.end(); iter++)
	{
		if ((*iter) != element) continue;
		elements.erase(iter);
		break;
	}
}