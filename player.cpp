#include "player.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <json/json.h>

#include "actionsfighter.h"

#include "graphics/texturemanager.h"
#include "debug.h"
#include "standardsprite.h"

Player::Player(std::size_t playerID)
	: m_playerID(playerID),
	  m_state(IdleState)
{
}

void Player::update()
{
	const double Acceleration = 4.0;
	const double Friction = 1.0;
	const double MaxVelocity = 15.0;
	const double JumpVelocity = 50.0;
	const double Gravity = 3.0;

	Actions &state = *Actions::instance()[m_playerID];
	math::vec3d facingDirection;

	double currentAcc    = Acceleration;
	double currentFri    = Friction;
	double currentMaxVel = MaxVelocity;
	double currentGrav   = Gravity;


	switch(m_state)
	{
	case IdleState:
		ensureTrack("idle");
		if (state.isDown(ActionsFighter::Attack)) {
			m_state = AttackState;
		} else if (state.isPressed(ActionsFighter::Block)) {
			m_state = DefendState;
		} else if (state.isPressed(ActionsFighter::Jump)) {
			m_velocity.y = JumpVelocity;
			m_state = JumpState;
		}
		else if (state.isPressed(ActionsFighter::Right) ^ state.isPressed(ActionsFighter::Left) ||
			 state.isPressed(ActionsFighter::Up   ) ^ state.isPressed(ActionsFighter::Down)) {
			m_state = RunState;
		}
		break;
	case AttackState:
		ensureTrack("attack");
		break;
	case DefendState:
		ensureTrack("defend");
		break;
	case JumpState:
		ensureTrack("jump");
		if (position().y < 0.1 && m_velocity.y <= 0)
			m_state = StandUpState;

		break;
	case StandUpState:
		ensureTrack("standup");
		break;
	case RunState:
	{
		ensureTrack("run");

		bool somePressed = false;
		if (state.isPressed(ActionsFighter::Right) && !state.isPressed(ActionsFighter::Left)) {
			setFacingDirection(RightDirection);
			facingDirection.x = 1.0;
			somePressed = true;
		}
		else if (!state.isPressed(ActionsFighter::Right) && state.isPressed(ActionsFighter::Left)) {
			setFacingDirection(LeftDirection);
			facingDirection.x = -1.0;
			somePressed = true;
		}
		if (state.isPressed(ActionsFighter::Up) && !state.isPressed(ActionsFighter::Down)) {
			facingDirection.z = 1.0;
			somePressed = true;
		}
		else if (!state.isPressed(ActionsFighter::Up) && state.isPressed(ActionsFighter::Down)) {
			facingDirection.z = -1.0;
			somePressed = true;
		}

		if (!somePressed) {
			m_state = IdleState;
		}

		if (state.isDown(ActionsFighter::Attack)) {
			m_state = AttackState;
		} else if (state.isPressed(ActionsFighter::Block)) {
			m_state = DefendState;
		} else if (state.isPressed(ActionsFighter::Jump)) {
			m_velocity.x = JumpVelocity * facingDirection.x;
			m_velocity.y = JumpVelocity;
			m_velocity.z = JumpVelocity * facingDirection.z;
			m_state = JumpState;
		}

		break;
	}
	default:
		m_state = IdleState;
		break;
	}

	if (facingDirection.module() > 0) {
		m_velocity += facingDirection.normalized() * currentAcc;
	}

	position() += m_velocity;

	if (position().y < 0.1) {
		position().y = 0;

		if (m_velocity.x > 0.0) {
			m_velocity.x = std::max(0.0, m_velocity.x - currentFri);
		}
		else {
			m_velocity.x = std::min(0.0, m_velocity.x + currentFri);
		}
		m_velocity.x = std::max(std::min(m_velocity.x, currentMaxVel), -currentMaxVel);

		if (m_velocity.z > 0.0) {
			m_velocity.z = std::max(0.0, m_velocity.z - currentFri);
		}
		else {
			m_velocity.z = std::min(0.0, m_velocity.z + currentFri);
		}
		m_velocity.z = std::max(std::min(m_velocity.z, currentMaxVel), -currentMaxVel);
	} else {
		m_velocity.y -= currentGrav;
	}

	AnimatedSprite::update();
}

std::shared_ptr<Player::Library> Player::loadDirectory(const std::string &path)
{
	std::shared_ptr<Library> lib = std::make_shared<Library>();
	loadTrackFile(path+"/idle.json", "idle", lib);
	loadTrackFile(path+"/attack.json", "attack", lib);
	loadTrackFile(path+"/jump.json", "jump", lib);
	loadTrackFile(path+"/standup.json", "standup", lib);
	loadTrackFile(path+"/run.json", "run", lib);
	loadTrackFile(path+"/lose.json", "lose", lib);
	loadTrackFile(path+"/defend.json", "defend", lib);
	loadTrackFile(path+"/throw.json", "throw", lib);
	loadTrackFile(path+"/victory.json", "victory", lib);

	return lib;
}

void Player::setFacingDirection(Player::HorizontalDirection direction)
{
	if (direction == LeftDirection) {
		setScale(math::vec2d(-1, 1));
		return;
	}

	setScale(math::vec2d(1, 1));
}

void Player::onCollisionLeft()
{
	m_velocity.x = 0;
}

void Player::onCollisionRight()
{
	m_velocity.x = 0;
}

void Player::onCollisionTop()
{
	m_velocity.z = 0;
}

void Player::onCollisionBottom()
{
	m_velocity.z = 0;
}

void Player::currentTrackFinished()
{
	switch(m_state)
	{
	case IdleState: break;
	case AttackState:
		m_state = IdleState;
		break;
	case DefendState:
		m_state = IdleState;
		break;
	case JumpState: break;
	case StandUpState:
		m_state = IdleState;
		break;
	case RunState: break;
	}
}
