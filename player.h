#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>

#include "math/bbox.h"
#include "animatedsprite.h"

class StandardSprite;

class Player : public AnimatedSprite
{
public:
	enum HorizontalDirection
	{
		LeftDirection,
		RightDirection
	};

	enum VerticalDirection
	{
		UpDirection,
		DownDirection
	};

	Player(std::size_t playerID = 0);

	virtual void update();

	void setFacingDirection(HorizontalDirection direction);

	void onCollisionLeft();
	void onCollisionRight();

	void onCollisionTop();
	void onCollisionBottom();

	static std::shared_ptr<Player::Library> loadDirectory(const std::string &path);

private:
	std::size_t m_playerID;

	math::vec3d m_velocity;

	//std::size_t m_idleTrack;

	void nextFrame();
	void nextKeyframeTime();

	// AnimatedSprite interface
protected:
	void currentTrackFinished();
};
