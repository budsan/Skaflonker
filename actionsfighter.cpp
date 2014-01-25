#include "actionsfighter.h"

Action guard;

ActionsFighter::ActionsFighter() : Actions(ActionCount)
{

}

void ActionsFighter::onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	getActionFromKey(unicode, key, mods).actionUp();
}

void ActionsFighter::onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{

	getActionFromKey(unicode, key, mods).actionDown();
}

Action& ActionsFighterOne::getActionFromKey(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	switch(key)
	{
	case Guy::Keyboard::Key_Up:      return m_actions[Up]; break;
	case Guy::Keyboard::Key_Down:    return m_actions[Down]; break;
	case Guy::Keyboard::Key_Left:    return m_actions[Left]; break;
	case Guy::Keyboard::Key_Right:   return m_actions[Right]; break;
	case Guy::Keyboard::Key_N:       return m_actions[Attack]; break;
	case Guy::Keyboard::Key_M:       return m_actions[Jump]; break;
	case Guy::Keyboard::Key_Comma:   return m_actions[Block]; break;
	case Guy::Keyboard::Key_Period:  return m_actions[Special]; break;
	default: return guard; break;
	}
}

Action& ActionsFighterTwo::getActionFromKey(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	switch(key)
	{
	case Guy::Keyboard::Key_W:       return m_actions[Up]; break;
	case Guy::Keyboard::Key_S:       return m_actions[Down]; break;
	case Guy::Keyboard::Key_A:       return m_actions[Left]; break;
	case Guy::Keyboard::Key_D:       return m_actions[Right]; break;
	case Guy::Keyboard::Key_Z:       return m_actions[Attack]; break;
	case Guy::Keyboard::Key_X:       return m_actions[Jump]; break;
	case Guy::Keyboard::Key_C:       return m_actions[Block]; break;
	case Guy::Keyboard::Key_V:       return m_actions[Special]; break;
	default: return guard; break;
	}
}
