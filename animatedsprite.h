#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>

#include "math/bbox.h"
#include "sprite.h"

class AnimatedSprite : public Sprite
{
public:
	struct Keyframe
	{
		std::string filename;
		math::bbox2i rect;
		math::vec2d origin;
		std::size_t frame;

		bool useAsVelocity;
		math::vec2d acceleration;
		math::vec2d friction;
		math::vec2d maxVel;

		std::vector<math::bbox2i> body;
		std::vector<math::bbox2i> damage;
	};

	struct Track
	{
		std::size_t frameCount;
		std::vector<Keyframe> keyframes;
	};

	struct Library
	{
		std::vector<Track> tracks;
		std::map<std::string, std::size_t> trackNames;
	};

	AnimatedSprite();

	virtual void update();

	math::vec2d projectedPosition() const;
	math::vec2d floorPosition() const;

	bool setLibrary(std::shared_ptr<Library> data);
	std::shared_ptr<Library> library() { return m_lib; }

	std::size_t currentTrack() const {return m_currentTrack;}
	std::size_t trackID(const std::string &name);
	bool ensureTrack(const std::string &name);
	bool playTrack(const std::string &name);
	bool playTrack(std::size_t animID);

	void drawParameters(Sprite::DrawParameters &params) override final;

protected:
	static bool loadTrackFile(const std::string& file, const std::string& name, std::shared_ptr<Library>& lib);

private:
	std::shared_ptr<Library> m_lib;

public:
	std::size_t m_playerID;
	std::size_t m_currentTrack;
	std::size_t m_currentKeyframe;
	std::size_t m_nextKeyframeTime;
	std::size_t m_trackFrame;

protected:
	virtual void currentTrackFinished() {}

private:
	void nextFrame();
	void nextKeyframeTime();
};

#endif // ANIMATEDSPRITE_H
