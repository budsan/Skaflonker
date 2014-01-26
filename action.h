#pragma once

#include <vector>

struct Action
{
	Action();

	void actionUp();
	void actionDown();

	void update(double deltaTime);
	void endOfFrame();

	bool isDown;
	bool isUp;
	bool isPressed;
	bool isDoubleDown;

	double lastTimePressed;
};

class Actions
{
public:
	Actions(int actionCount);
	~Actions();

	static std::vector<Actions*>& instance();

	static void updateAll(double deltaTime);
	static void endOfFrameAll();

	bool isDown(unsigned int type) const;
	bool isUp(unsigned int type) const;
	bool isPressed(unsigned int type) const;

	void update(double deltaTime);
	void endOfFrame();



protected:

	std::vector<Action> m_actions;

private:
	static std::vector<Actions*> s_instance;
};

