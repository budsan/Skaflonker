#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>

#include "math/bbox.h"
#include "sprite.h"

class Player : public Sprite
{
public:

	struct Keyframe
	{
		std::string filename;
		math::bbox2i rect;
		math::vec2d origin;
		std::size_t frame;

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

	Player();
	Player(std::shared_ptr<Library> data);

	virtual void update();

	bool loadDirectory(const std::string &path);

	std::size_t trackID(const std::string &name);

	bool setLibrary(std::shared_ptr<Library> data);
	bool playTrack(const std::string &name);
	bool playTrack(std::size_t animID);

	void drawParameters(Sprite::DrawParameters &params) override;

private:
	bool loadTrackFile(const std::string& file, const std::string& name, std::shared_ptr<Library>& lib);

	std::shared_ptr<Library> m_lib;

	std::size_t m_currentTrack;
	std::size_t m_currentKeyframe;
	std::size_t m_nextKeyframeTime;
	std::size_t m_trackFrame;

	void nextFrame();
	void nextKeyframeTime();
};
