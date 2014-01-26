#include "action.h"
#include <cstdio>

Action::Action() : isDown(false), isUp(false), isPressed(false), isDoubleDown(false), lastTimePressed(-1)
{

}

void Action::actionUp()
{
	isUp = true;
	isPressed = false;
}

void Action::actionDown()
{
	isDown = true;
	isPressed = true;
	if (lastTimePressed >= 0 && lastTimePressed < 0.25) {
		isDoubleDown = true;
		lastTimePressed = -1;
	} else {
		lastTimePressed = 0;
	}
}

void Action::update(double deltaTime)
{
	if (lastTimePressed >= 0)
		lastTimePressed += deltaTime;
}

void Action::endOfFrame()
{
	isDown = false;
	isDoubleDown = false;
	isUp = false;
}

std::vector<Actions*> Actions::s_instance;
Actions::Actions(int actionCount) : m_actions(actionCount)
{
}

Actions::~Actions()
{
}

std::vector<Actions*>& Actions::instance()
{
	return s_instance;
}

void Actions::updateAll(double deltaTime)
{
	for (unsigned int i = 0; i < s_instance.size(); ++i) {
		s_instance[i]->update(deltaTime);
	}
}

void Actions::endOfFrameAll()
{
	for (unsigned int i = 0; i < s_instance.size(); ++i) {
		s_instance[i]->endOfFrame();
	}
}

bool Actions::isDown(unsigned int type) const
{
	return m_actions[(unsigned int)type].isDown;
}

bool Actions::isUp(unsigned int type) const
{
	return m_actions[(unsigned int)type].isUp;
}

bool Actions::isPressed(unsigned int type) const
{
	return m_actions[(unsigned int)type].isPressed;
}

void Actions::update(double deltaTime)
{
	for (unsigned int i = 0; i < m_actions.size(); ++i)
	{
		m_actions[i].update(deltaTime);
	}
}

void Actions::endOfFrame()
{
	for (unsigned int i = 0; i < m_actions.size(); ++i)
	{
		m_actions[i].endOfFrame();
	}
}
