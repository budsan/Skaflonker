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
	: m_playerID(playerID)
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

	// Uncomment the line below to make jumps more real :(
	if (currentTrack() != trackID("attack") /*&& position().y < 0.1*/) {
		if (state.isDown(ActionsFighter::Attack)) {
			ensureTrack("attack");
		} else if (state.isPressed(ActionsFighter::Block)) {
			ensureTrack("defend");
		} else if (state.isPressed(ActionsFighter::Jump)) {
			m_velocity.y = JumpVelocity;
		} else {
			if (!state.isPressed(ActionsFighter::Right) && !state.isPressed(ActionsFighter::Left)
				&& !state.isPressed(ActionsFighter::Up) && !state.isPressed(ActionsFighter::Down))
			{
				ensureTrack("idle");
			} else {
				math::vec3d facingDirection;
				if (state.isPressed(ActionsFighter::Right)) {
					ensureTrack("run");
					setFacingDirection(RightDirection);
					facingDirection.x = 1.0;
				} else if (state.isPressed(ActionsFighter::Left)) {
					ensureTrack("run");
					setFacingDirection(LeftDirection);
					facingDirection.x = -1.0;
				}

				if (state.isPressed(ActionsFighter::Up)) {
					ensureTrack("run");
					facingDirection.z = 1.0;
				} else if (state.isPressed(ActionsFighter::Down)) {
					ensureTrack("run");
					facingDirection.z = -1.0;
				}

				m_velocity += facingDirection.normalized() * Acceleration;
			}
		}
	}

	position() += m_velocity;

	if (position().y < 0.1) {
		position().y = 0;

		if (m_velocity.x > 0.0) {
			m_velocity.x = std::max(0.0, m_velocity.x - Friction);
		}
		else {
			m_velocity.x = std::min(0.0, m_velocity.x + Friction);
		}
		m_velocity.x = std::max(std::min(m_velocity.x, MaxVelocity), -MaxVelocity);

		if (m_velocity.z > 0.0) {
			m_velocity.z = std::max(0.0, m_velocity.z - Friction);
		}
		else {
			m_velocity.z = std::min(0.0, m_velocity.z + Friction);
		}
		m_velocity.z = std::max(std::min(m_velocity.z, MaxVelocity), -MaxVelocity);
	} else {
		m_velocity.y -= Gravity;
	}

	AnimatedSprite::update();
}

std::shared_ptr<Player::Library> Player::loadDirectory(const std::string &path)
{
	std::shared_ptr<Library> lib = std::make_shared<Library>();
	if (!loadTrackFile(path+"/idle.json", "idle", lib)) return std::shared_ptr<Library>();
	if (!loadTrackFile(path+"/attack.json", "attack", lib)) return std::shared_ptr<Library>();
	if (!loadTrackFile(path+"/run.json", "run", lib)) return std::shared_ptr<Library>();
	if (!loadTrackFile(path+"/lose.json", "lose", lib)) return std::shared_ptr<Library>();
	if (!loadTrackFile(path+"/defend.json", "defend", lib)) return std::shared_ptr<Library>();
	if (!loadTrackFile(path+"/throw.json", "throw", lib)) return std::shared_ptr<Library>();
	if (!loadTrackFile(path+"/victory.json", "victory", lib)) return std::shared_ptr<Library>();

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
	m_velocity.y = 0;
}

void Player::onCollisionBottom()
{
	m_velocity.y = 0;
}

void Player::currentTrackFinished()
{
	if (currentTrack() != trackID("run") && currentTrack() != trackID("defend"))
		ensureTrack("idle");
}
