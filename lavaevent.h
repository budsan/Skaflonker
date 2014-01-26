#ifndef LAVAEVENT_H
#define LAVAEVENT_H

#include "environmentevent.h"

class LavaEvent : public EnvironmentEvent
{
public:
	LavaEvent();

	// EnvironmentEvent interface
public:
	bool isExpired() const;
	const std::string text() const;
};

#endif // LAVAEVENT_H
