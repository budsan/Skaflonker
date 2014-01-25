#ifndef STANDARDSPRITE_H
#define STANDARDSPRITE_H

#include "sprite.h"

#include <string>

class StandardSprite : public Sprite
{
public:
	StandardSprite(const std::string &fileName = std::string());

	const std::string &fileName() const;

	void setFileName(const std::string &fileName);
	void setPosition(const math::vec2d &position);
	void setOffset(const math::vec2d &offset);

	// Sprite interface
protected:
	void drawParameters(Sprite::DrawParameters &params);
	void preDrawing();
	void postDrawing();

private:
	std::string m_fileName;
	math::vec2d m_offset;
};

#endif // STANDARDSPRITE_H
