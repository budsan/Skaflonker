#ifndef ENVIRONMENTEVENT_H
#define ENVIRONMENTEVENT_H

#include <string>

class EnvironmentEvent
{
public:
	EnvironmentEvent();

	virtual bool isExpired() const = 0;
	virtual const std::string text() const = 0;
};

#endif // ENVIRONMENTEVENT_H
