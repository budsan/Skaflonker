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

	Player(std::size_t m_playerID = 0);

    virtual void update(double delta);

	std::size_t trackID(const std::string &name);

	bool setLibrary(std::shared_ptr<Library> data);
	std::shared_ptr<Library> library() { return m_lib; }

	bool ensureTrack(const std::string &name);

	bool playTrack(const std::string &name);
	bool playTrack(std::size_t animID);

	void drawParameters(Sprite::DrawParameters &params) override;

	static std::shared_ptr<Library> loadDirectory(const std::string &path);

    void setFacingDirection(HorizontalDirection direction);
    void moveHorizontallyTo(HorizontalDirection direction, double delta);
    void moveVerticallyTo(VerticalDirection direction, double delta);

private:
	static bool loadTrackFile(const std::string& file, const std::string& name, std::shared_ptr<Library>& lib);

	std::shared_ptr<Library> m_lib;

	std::size_t m_playerID;
	std::size_t m_currentTrack;
	std::size_t m_currentKeyframe;
	std::size_t m_nextKeyframeTime;
	std::size_t m_trackFrame;

//    std::size_t m_idleTrack;

	void nextFrame();
	void nextKeyframeTime();
};
