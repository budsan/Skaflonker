#include "animatedsprite.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

#include "debug.h"

AnimatedSprite::AnimatedSprite()
: Sprite(),
  m_currentTrack(0),
  m_currentKeyframe(0),
  m_nextKeyframeTime(0),
  m_trackFrame(0)
{

}

AnimatedSprite::AnimatedSprite(std::shared_ptr<Library> _data)
{
	setLibrary(_data);
}

void AnimatedSprite::update()
{
	if (m_lib == nullptr) return;
	if (++m_trackFrame >= m_nextKeyframeTime) {
		nextFrame();
	}
}

void AnimatedSprite::nextFrame()
{
	const Track& track = m_lib->tracks[m_currentTrack];
	if (m_currentTrack+1 < track.keyframes.size()) {
		m_currentTrack++;
	}
	else {
		m_currentTrack = 0;
	}

	nextKeyframeTime();
}

void AnimatedSprite::nextKeyframeTime()
{
	const Track& track = m_lib->tracks[m_currentTrack];
	if (m_currentTrack+1 < track.keyframes.size()) {
		m_nextKeyframeTime = track.keyframes[m_currentTrack+1].frame;
	}
	else {
		m_nextKeyframeTime = track.frameCount;
	}
}

bool AnimatedSprite::setLibrary(std::shared_ptr<Library> _data)
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

	return true;
}

void AnimatedSprite::drawParameters(Sprite::DrawParameters &params)
{
	if (m_lib == nullptr) {
		params.filename = nullptr;
		return;
	}

	const Track& track = m_lib->tracks[m_currentTrack];
	const Keyframe& key = track.keyframes[m_currentKeyframe];

	params.filename = key.filename.c_str();
	params.x  = key.bottomLeft.x;
	params.y  = key.bottomLeft.y;
	params.w  = key.topRight.x - key.bottomLeft.x;
	params.h  = key.topRight.y - key.bottomLeft.y;
	params.cx = key.origin.x;
	params.cy = key.origin.y;
}

std::size_t AnimatedSprite::trackID(const std::string &name)
{
	if (m_lib == nullptr)
		return 0;

	auto it = m_lib->trackNames.find(name);
	if (it == m_lib->trackNames.end()) return m_lib->tracks.size();
	return it->second;
}

bool AnimatedSprite::playTrack(const std::string &name)
{
	if (m_lib == nullptr)
		return false;

	std::size_t animID = trackID(name);
	return playTrack(animID);
}

bool AnimatedSprite::playTrack(std::size_t animID)
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
