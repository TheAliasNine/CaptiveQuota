#pragma once

#include "raylib.h"

#include <fstream>

#define CONTROLFILENAME "controls.ini"

class InputManager
{
public:
	enum KeyBind
	{
		KEYNULL,
		//Basic Navigation
		Confirm,
		Back,
		Up,
		Down,
		Left,
		Right,
		Cast,
		OpenMenu,

		KEYBINDLENGTH,
	};
private:

	struct KeyBindInfo
	{
		KeyBind action;
		int key = 0;

		bool enabled = false;

		KeyBindInfo() { action = KEYNULL; }
		KeyBindInfo(KeyBind key) { this->action = key; }
	};

	class KeyBinds
	{
		//array holding all key bind info
		KeyBindInfo binds[KEYBINDLENGTH];

		void Load();

	public:
		KeyBinds()
		{
			for (int i = KEYNULL + 1; i < KEYBINDLENGTH; i++)
			{
				binds[i] = KeyBindInfo(static_cast<KeyBind>(i));
			}
			Load();
		}
		KeyBindInfo* operator[] (int i)
		{
			return &binds[i];
		}

		void LoadDefaults();

		bool LoadByFile();


		bool SaveBinds();
	};

	static KeyBinds keyBinds;

	//enabled back up
	static bool enabledBU[KEYBINDLENGTH];

	//makes class unable to be instantiated
private: InputManager() {}
	


	//Interactable Parts
public:
	static bool KeyPressed(KeyBind bind)
	{
		if (!keyBinds[bind]->enabled) return false;
		return IsKeyPressed(keyBinds[bind]->key) || IsMouseButtonPressed(keyBinds[bind]->key);
	}
	static bool KeyDown(KeyBind bind)
	{
		if (!keyBinds[bind]->enabled) return false;
		return IsKeyDown(keyBinds[bind]->key) || IsMouseButtonDown(keyBinds[bind]->key);
	}
	static bool KeyReleased(KeyBind bind)
	{
		if (!keyBinds[bind]->enabled) return false;
		return IsKeyReleased(keyBinds[bind]->key) || IsMouseButtonReleased(keyBinds[bind]->key);
	}


	static void SetKey(KeyBind bind, int i)
	{
		keyBinds[bind]->key = i;
	}

	static void BackUpEnabled();

	static void RestoreBackUps();

	static void DisableAll();

	static void LoadDefaults() { keyBinds.LoadDefaults(); }

	static void SaveBinds() { keyBinds.SaveBinds(); }

	static void EnableBind(int i) { keyBinds[i]->enabled = true; }
	static void DisableBind(int i) { keyBinds[i]->enabled = false; }
};