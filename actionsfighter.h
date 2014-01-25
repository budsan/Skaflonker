#pragma once

#include "action.h"
#include "guyframework/input/keyboardlistener.h"

class ActionsFighter : public Actions, public Guy::KeyboardListener
{
public:
	ActionsFighter();

	enum Type { Up, Down, Left, Right, Attack, Jump, Block, Special, ActionCount };

	void onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
	void onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);

	virtual Action& getActionFromKey(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods) = 0;
};

class ActionsFighterOne : public ActionsFighter
{
public:
	virtual Action& getActionFromKey(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
};

class ActionsFighterTwo : public ActionsFighter
{
public:
	virtual Action& getActionFromKey(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
};
