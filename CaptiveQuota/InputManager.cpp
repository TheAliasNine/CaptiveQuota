#include "InputManager.h"

InputManager::KeyBinds InputManager::keyBinds = KeyBinds();

bool InputManager::enabledBU[InputManager::KEYBINDLENGTH] = {false};

void InputManager::BackUpEnabled()
{
	for (int i = 0; i < KEYBINDLENGTH; i++)
	{
		enabledBU[i] = keyBinds[i]->enabled;
	}
}

void InputManager::RestoreBackUps()
{
	for (int i = 0; i < KEYBINDLENGTH; i++)
	{
		keyBinds[i]->enabled = enabledBU[i];
	}
}

void InputManager::DisableAll()
{
	for (int i = 0; i < KEYBINDLENGTH; i++)
	{
		keyBinds[i]->enabled = false;
	}
}


void InputManager::KeyBinds::Load()
{
	if (!LoadByFile())
		LoadDefaults();
}

void InputManager::KeyBinds::LoadDefaults()
{
	binds[Confirm].key = MOUSE_LEFT_BUTTON;
}

bool InputManager::KeyBinds::LoadByFile()
{
	std::fstream file;
	file.open(CONTROLFILENAME, std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		for (int i = 1; i < KEYBINDLENGTH; i++)
		{
			file.read((char*)&binds[i].key, sizeof(int));
		}
		file.close();
		return true;
	}
	return false;
}

bool InputManager::KeyBinds::SaveBinds()
{
	std::fstream file;
	file.open(CONTROLFILENAME, std::ios::out | std::ios::binary | std::ios::trunc);
	if (file.is_open())
	{
		for (int i = 1; i < KEYBINDLENGTH; i++)
		{
			file.write((char*)&binds[i].key, sizeof(int));
		}
		file.close();
	}
	return true;
}