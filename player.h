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

	void dealDamage(int damage);

	void onDamageTaken(const Player &from);
	void onDamageGiven(const Player &to);

private:
	enum {
		IdleState,
		AttackState,
		DefendState,
		JumpState,
		AirAttackState,
		StandUpState,
		RunState,
		HurtState,
		LauncherState,
		LaunchedInitState,
		LaunchedState
	};

	int m_state;

	std::size_t m_playerID;
	math::vec3d m_velocity;
	int hitFramesRemaining;

	//std::size_t m_idleTrack;

	int m_health;

	void nextFrame();
	void nextKeyframeTime();

	void drawParameters(Sprite::DrawParameters &params) override final;

	// AnimatedSprite interface
protected:
	void currentTrackFinished();
};
