#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>

#include "sprite.h"

class AnimatedSprite : public Sprite
{
public:

	struct Keyframe
	{
		std::string filename;
		math::vec2d bottomLeft;
		math::vec2d topRight;
		math::vec2d origin;
		std::size_t frame;
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
	AnimatedSprite(std::shared_ptr<Library> data);

	virtual void update();

	std::size_t trackID(const std::string &name);

	bool setLibrary(std::shared_ptr<Library> data);
	bool playTrack(const std::string &name);
	bool playTrack(std::size_t animID);

	void drawParameters(Sprite::DrawParameters &params) override;

private:
	std::shared_ptr<Library> m_lib;

	std::size_t m_currentTrack;
	std::size_t m_currentKeyframe;
	std::size_t m_nextKeyframeTime;
	std::size_t m_trackFrame;

	void nextFrame();
	void nextKeyframeTime();
};
