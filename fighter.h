#pragma once

#include <string>
#include "animatedsprite.h"

class Fighter : public AnimatedSprite
{
public:
	Fighter();
	virtual ~Fighter();

	virtual void update();

	bool loadDirectory(const std::string &path);
private:

	bool loadTrackFile(const std::string& file, const std::string& name, std::shared_ptr<Library>& lib);

protected:
	math::vec2d m_acc;
	math::vec2d m_vel;
	math::vec2d m_fri;
	math::vec2d m_velLim;
	double m_animVelFactor;


	std::string m_lastAnim;
};
