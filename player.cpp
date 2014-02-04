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

constexpr int MaxHealth{100};

Player::Player(std::size_t playerID)
	: m_playerID(playerID),
	  m_health(MaxHealth),
	  m_state(IdleState),
	  hitFramesRemaining(0.0)
{
}

void Player::update()
{
	if (hitFramesRemaining > 0) {
		--hitFramesRemaining;
		return;
	}

	const double Acceleration = 4.0;
	const double Friction = 1.0;
	const double MaxVelocity = 15.0;
	const double JumpVelocity = 60.0;
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

		if (state.isPressed(ActionsFighter::Right) ^ state.isPressed(ActionsFighter::Left) ||
		    state.isPressed(ActionsFighter::Up   ) ^ state.isPressed(ActionsFighter::Down)) {
			m_state = RunState;
		}
		else
		{
			if (state.isDown(ActionsFighter::Attack)) {
				m_state = AttackState;
				facingDirection.x = scale().x;
				m_velocity.x = 15 * scale().x * 0.5;
			} else if (state.isPressed(ActionsFighter::Block)) {
				m_state = DefendState;
			} else if (state.isPressed(ActionsFighter::Jump)) {
				m_velocity.y = JumpVelocity;
				m_state = JumpState;
			}
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
		currentAcc = currentFri = 0;
		currentMaxVel = JumpVelocity / 2;

		if (state.isDown(ActionsFighter::Attack)) {
			m_state = AirAttackState;
		}

		if (position().y < 0.1 && m_velocity.y <= 0)
			m_state = StandUpState;

		break;
	case AirAttackState:
		ensureTrack("attack");
		currentAcc = currentFri = 0;
		currentMaxVel = JumpVelocity;

		if (position().y < 0.1 && m_velocity.y <= 0)
			m_state = StandUpState;

		break;
	case HurtState:
		ensureTrack("hurt");
		break;
	case LaunchedInitState:
	{
		m_state = LaunchedState;
		facingDirection.x = scale().x;
		m_velocity.x = JumpVelocity * scale().x * -0.5;
		m_velocity.y = JumpVelocity;
		m_velocity.z = 0;
	}
	case LaunchedState:
		currentAcc = currentFri = 0;
		currentMaxVel = JumpVelocity / 2;

		if (m_velocity.y > 0)
			ensureTrack("launchup");
		else
			ensureTrack("launchdown");

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

			currentAcc = currentFri = 0;
			currentMaxVel = JumpVelocity / 2;
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

	m_velocity.y -= currentGrav;
	position() += m_velocity;

	if (position().y < 0.1)
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


	AnimatedSprite::update();
}

std::shared_ptr<Player::Library> Player::loadDirectory(const std::string &path)
{
	std::shared_ptr<Library> lib = std::make_shared<Library>();
	loadTrackFile(path+"/idle.json", "idle", lib);
	loadTrackFile(path+"/attack.json", "attack", lib);
	loadTrackFile(path+"/attack-launcher.json", "launcher", lib);
	loadTrackFile(path+"/jump.json", "jump", lib);
	loadTrackFile(path+"/hurt.json", "hurt", lib);
	loadTrackFile(path+"/launchup.json", "launchup", lib);
	loadTrackFile(path+"/launchdown.json", "launchdown", lib);
	loadTrackFile(path+"/standup.json", "standup", lib);
	loadTrackFile(path+"/run.json", "run", lib);
	loadTrackFile(path+"/lose.json", "lose", lib);
	loadTrackFile(path+"/defend.json", "defend", lib);
	loadTrackFile(path+"/throw.json", "throw", lib);
	loadTrackFile(path+"/victory.json", "victory", lib);

	return lib;
}

void Player::dealDamage(int damage)
{
	m_health = std::max(0, m_health - std::abs(damage));
	if (m_health == 0) {
		// Player just dead
	}
}

void Player::onDamageTaken(const Player &from)
{
	switch(m_state)
	{
	case DefendState:
		hitFramesRemaining = 5;
		break;
	case LaunchedState:
		m_state = LaunchedInitState;
		setFacingDirection(from.scale().x > 0 ? LeftDirection : RightDirection);
		ensureTrack("launchup");
		hitFramesRemaining = 5;
		break;
	default:
		switch(from.m_state)
		{
			case AttackState:
				m_state = HurtState;
				ensureTrack("hurt");
				hitFramesRemaining = 10;
				break;
			case LauncherState:
			case AirAttackState:
				m_state = LaunchedInitState;
				setFacingDirection(from.scale().x > 0 ? LeftDirection : RightDirection);
				ensureTrack("launchup");
				hitFramesRemaining = 5;
				break;
		}
		break;
	}
}

void Player::onDamageGiven(const Player &to)
{

}

void Player::drawParameters(Sprite::DrawParameters &params)
{
	AnimatedSprite::drawParameters(params);

	if (hitFramesRemaining>0) {
		params.cx += (rand()%20)-10;
		params.cy += (rand()%20)-10;
	}
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
	Actions &state = *Actions::instance()[m_playerID];
	switch(m_state)
	{
	case IdleState:
		break;
	case AttackState:
		m_state = IdleState;
		break;
	case DefendState:
		if (!state.isPressed(ActionsFighter::Block))
			m_state = IdleState;
		break;
	case JumpState:
		break;
	case HurtState:
		if (state.isPressed(ActionsFighter::Block))
			m_state = DefendState;
		else
			m_state = IdleState;
		break;
	case AirAttackState:
		m_state = JumpState;
		break;
	case StandUpState:
		m_state = IdleState;
		break;
	case RunState:
		break;
	}
}
