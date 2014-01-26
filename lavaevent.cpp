#include "lavaevent.h"

LavaEvent::LavaEvent()
{
}

bool LavaEvent::isExpired() const
{
	return false;
}

const std::string LavaEvent::text() const
{
	return "test";
}
