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

Player::Player(std::size_t playerID)
	: Sprite(),
	  m_currentTrack(0),
	  m_currentKeyframe(0),
	  m_nextKeyframeTime(0),
      m_trackFrame(0),
      m_playerID(playerID)
{

}

void Player::update(double /*delta*/)
{
    const double Acceleration = 4.0;
    const double Friction = 1.0;
    const double MaxVelocity = 15.0;

	Actions &state = *Actions::instance()[m_playerID];

    if (m_currentTrack != trackID("attack")) {
        if (state.isDown(ActionsFighter::Attack)) {
            ensureTrack("attack");
        } else if (state.isPressed(ActionsFighter::Block)) {
            ensureTrack("defend");
        } else {
            if (!state.isPressed(ActionsFighter::Right) && !state.isPressed(ActionsFighter::Left)
                    && !state.isPressed(ActionsFighter::Up) && !state.isPressed(ActionsFighter::Down))
            {
                ensureTrack("idle");
            } else {
                math::vec2d facingDirection;
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
                    facingDirection.y = 1.0;
                } else if (state.isPressed(ActionsFighter::Down)) {
                    ensureTrack("run");
                    facingDirection.y = -1.0;
                }

                m_velocity += facingDirection.normalized() * Acceleration;
            }
        }
    }

    position() += m_velocity;
//    if (m_velocity.x > 0.0) {
//        m_velocity.x = std::min(std::max(0.0, m_velocity.x - Friction), MaxVelocity);
//    }
//    else {
//        m_velocity.x = std::max(std::min(0.0, m_velocity.x + Friction), -MaxVelocity);
//    }
//    if (m_velocity.y > 0.0) {
//        m_velocity.y = std::max(std::max(0.0, m_velocity.y - Friction), MaxVelocity);
//    }
//    else {
//        m_velocity.y = std::min(std::min(0.0, m_velocity.y + Friction), -MaxVelocity);
//    }
    if (m_velocity.x > 0.0) {
        m_velocity.x = std::max(0.0, m_velocity.x - Friction);
    }
    else {
        m_velocity.x = std::min(0.0, m_velocity.x + Friction);
    }
    if (m_velocity.y > 0.0) {
        m_velocity.y = std::max(0.0, m_velocity.y - Friction);
    }
    else {
        m_velocity.y = std::min(0.0, m_velocity.y + Friction);
    }

    m_velocity.x = std::max(std::min(m_velocity.x, MaxVelocity), -MaxVelocity);
    m_velocity.y = std::max(std::min(m_velocity.y, MaxVelocity), -MaxVelocity);

	if (m_lib == nullptr) return;
	if (++m_trackFrame >= m_nextKeyframeTime) {
		nextFrame();
	}
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

bool Player::loadTrackFile(const std::string& file, const std::string& name, std::shared_ptr<Library> &lib)
{
	std::ifstream sfile(file.c_str());
	if (!sfile.is_open()) {
		Guy::printLog("Error loading %s anim file", file.c_str());
		return false;
	}

	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(sfile, root) ) {
		Guy::printLog("Failed to parse %s anim file, reason: %s", file.c_str(), reader.getFormatedErrorMessages().c_str());
		return false;
	}

	Track track;
	track.frameCount = root.get("frameCount", "UTF-8" ).asUInt();
	const Json::Value keyframes = root["keyFrames"];
	for ( int index = 0; index < keyframes.size(); ++index )
	{
		Keyframe key;
		const Json::Value keyframe = keyframes[index];
		const Json::Value data = keyframe["data"];
		key.frame = keyframe.get("frame", "UTF-8").asUInt();
		key.filename = data.get("fileName", "UTF-8").asString();
		Guy::TextureManager::instance().getTexture(key.filename);

		const Json::Value offset = data["offset"];
		key.origin.x = -offset.get("x", "UTF-8").asDouble();
		key.origin.y = -offset.get("y", "UTF-8").asDouble();

		{
			const Json::Value rect = data["rect"];
			const Json::Value bottomRight = rect["bottomRight"];
			const Json::Value topLeft     = rect["topLeft"];

			key.rect.update(math::vec2i(bottomRight.get("x", "UTF-8").asInt(), bottomRight.get("y", "UTF-8").asInt()));
			key.rect.update(math::vec2i(    topLeft.get("x", "UTF-8").asInt(),     topLeft.get("y", "UTF-8").asInt()));
		}

		{
			const Json::Value custom = data["customProperties"];
			Json::Value::Members members = custom.getMemberNames();
			for(std::size_t i; i < members.size(); ++i) {
				if (members[i] == std::string("velx"))
				{
					key.acceleration.x = custom.get("velx", "UTF-8").asDouble();
					key.useAsVelocity = true;
				}
				else if (members[i] == std::string("vely"))
				{
					key.acceleration.y = custom.get("vely", "UTF-8").asDouble();
					key.useAsVelocity = true;
				}
				else if (members[i] == std::string("accx"))
				{
					key.acceleration.x = custom.get("accx", "UTF-8").asDouble();
					key.useAsVelocity = false;
				}
				else if (members[i] == std::string("accy"))
				{
					key.acceleration.y = custom.get("accy", "UTF-8").asDouble();
					key.useAsVelocity = false;
				}
				else if (members[i] == std::string("frix"))
				{
					key.friction.x = custom.get("frix", "UTF-8").asDouble();
				}
				else if (members[i] == std::string("friy"))
				{
					key.friction.y = custom.get("friy", "UTF-8").asDouble();
				}
				else if (members[i] == std::string("maxx"))
				{
					key.maxVel.x = custom.get("maxx", "UTF-8").asDouble();
				}
				else if (members[i] == std::string("maxy"))
				{
					key.maxVel.y = custom.get("maxy", "UTF-8").asDouble();
				}
			}
		}

		const Json::Value hitBoxes = data["hitBoxes"];
		for ( int index = 0; index < hitBoxes.size(); ++index )
		{
			const Json::Value hitBox = hitBoxes[index];
			const Json::Value rect = hitBox["rect"];
			const Json::Value bottomRight = rect["bottomRight"];
			const Json::Value topLeft     = rect["topLeft"];

			math::bbox2i box;
			box.update(math::vec2i(bottomRight.get("x", "UTF-8").asInt(), bottomRight.get("y", "UTF-8").asInt()));
			box.update(math::vec2i(    topLeft.get("x", "UTF-8").asInt(),     topLeft.get("y", "UTF-8").asInt()));

			if (hitBox.get("group", "UTF-8").asString() == std::string("body")) {
				key.body.push_back(box);
			}
			if (hitBox.get("group", "UTF-8").asString() == std::string("damage")) {
				key.damage.push_back(box);
			}
		}

		track.keyframes.push_back(key);
	}

	lib->trackNames.insert(std::make_pair(name, lib->tracks.size()));
	lib->tracks.push_back(track);

	return true;
}

bool Player::setLibrary(std::shared_ptr<Library> _data)
{
	if (_data == nullptr)
	{
		m_lib.reset();
		return true;
	}

	if (_data->tracks.size() == 0)
		return false;

	m_lib = _data;
	if (!playTrack(0)) {
		m_lib.reset();
		return false;
	}

//    m_idleTrack = trackID("idle");

	return true;
}

void Player::drawParameters(Sprite::DrawParameters &params)
{
	if (m_lib == nullptr) {
        params.filename = nullptr;
		return;
	}

	const Track& track = m_lib->tracks[m_currentTrack];
	const Keyframe& key = track.keyframes[m_currentKeyframe];

	params.filename = key.filename.c_str();
	params.x  = key.rect.min.x;
	params.y  = key.rect.min.y;
	params.w  = key.rect.max.x - key.rect.min.x;
	params.h  = key.rect.max.y - key.rect.min.y;
	params.cx = key.origin.x;
	params.cy = key.origin.y;
}

std::size_t Player::trackID(const std::string &name)
{
	if (m_lib == nullptr)
		return 0;

	auto it = m_lib->trackNames.find(name);
	if (it == m_lib->trackNames.end()) return m_lib->tracks.size();
	return it->second;
}

bool Player::ensureTrack(const std::string &name)
{
	if (m_lib == nullptr)
		return false;

	std::size_t animID = trackID(name);
	if (animID != m_currentTrack)
		return playTrack(animID);

	return true;
}

bool Player::playTrack(const std::string &name)
{
	if (m_lib == nullptr)
		return false;

	std::size_t animID = trackID(name);
	return playTrack(animID);
}

bool Player::playTrack(std::size_t animID)
{
	if (m_lib == nullptr)
		return false;

	if (animID >= m_lib->tracks.size())
		return false;

	if (m_lib->tracks[animID].keyframes.size() == 0)
		return false;

	m_currentTrack = animID;
	m_currentKeyframe = 0;
	m_trackFrame = m_lib->tracks[m_currentTrack].keyframes[m_currentKeyframe].frame;
	nextKeyframeTime();

	return true;
}

void Player::nextFrame()
{
	const Track& track = m_lib->tracks[m_currentTrack];
	if (m_currentKeyframe+1 < track.keyframes.size()) {
		m_currentKeyframe++;
	}
	else {
		m_currentKeyframe = 0;
        if (m_currentTrack != trackID("run") && m_currentTrack != trackID("defend"))
            ensureTrack("idle");
		m_trackFrame = m_lib->tracks[m_currentTrack].keyframes[m_currentKeyframe].frame;
	}

	nextKeyframeTime();
}

void Player::nextKeyframeTime()
{
	const Track& track = m_lib->tracks[m_currentTrack];
	if (m_currentKeyframe+1 < track.keyframes.size()){
		m_nextKeyframeTime = track.keyframes[m_currentKeyframe+1].frame;
	}
	else {
		m_nextKeyframeTime = track.frameCount;
	}
}
