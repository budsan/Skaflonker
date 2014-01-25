#include "standardsprite.h"

#include <graphics/texturemanager.h>
#include <graphics/texture.h>

StandardSprite::StandardSprite(const std::string &fileName)
{
	setFileName(fileName);
}

const std::string &StandardSprite::fileName() const
{
	return m_fileName;
}

void StandardSprite::setFileName(const std::string &fileName)
{
	m_fileName = fileName;
}

void StandardSprite::setPosition(const math::vec2d &position)
{
	this->position() = position;
}

void StandardSprite::setOffset(const math::vec2d &offset)
{
	m_offset = offset;
}

void StandardSprite::drawParameters(Sprite::DrawParameters &params)
{
	params.filename = m_fileName.c_str();
	const Guy::Texture &texture = Guy::TextureManager::instance().getTexture(m_fileName);
	params.x = 0;
	params.y = 0;
	params.w = texture.getWidth();
	params.h = texture.getHeight();
	params.cx = texture.getWidth() / 2.0;
	params.cy = texture.getHeight() / 2.0;
}

void StandardSprite::preDrawing()
{

}

void StandardSprite::postDrawing()
{

}
