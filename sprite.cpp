#include "sprite.h"

#include "environment.h"
#include "graphics/texturemanager.h"
#include "graphics/screen.h"
#include "graphics/color.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

double Sprite::s_globalUnitsPerPixel = 1.0;

Sprite::Sprite()
{
	m_unitsPerPixel = s_globalUnitsPerPixel;
	m_scale.x = 1.0;
	m_scale.y= 1.0;
	m_rotate = 0.0;
}

Sprite::~Sprite()
{

}

void Sprite::setGlobalPixelsPerUnit(double value)
{
	s_globalUnitsPerPixel = 1.0/value;
}

void Sprite::setPixelsPerUnit(double value)
{
	m_unitsPerPixel = 1.0/value;
}

void Sprite::setScale(double value)
{
	m_scale.x = value;
	m_scale.y = value;
}

void Sprite::setScale(math::vec2d value)
{
	m_scale = value;
}

void Sprite::setScaleWidth(double value)
{
	m_scale.x = value;
}

void Sprite::setScaleHeight(double value)
{
	m_scale.y = value;
}

void Sprite::setRotation(double value)
{
	m_rotate = value;
}

void Sprite::draw()
{
	DrawParameters params;
	drawParameters(params);
	if (params.filename == NULL) return;

	Guy::TextureManager *texman = Guy::TextureManager::ptrInstance();

	const Guy::Texture &tex = texman->getTexture(params.filename);
	tex.setFiltering(GL_NEAREST);
	tex.setWrap(GL_CLAMP_TO_EDGE);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_ED, rgba(0,0,0,0).raw());

	math::vec2i tSize = tex.getSize();
	struct { float x0, y0, x1, y1; } normCoords;
	normCoords.x0 = (float)params.x/(float)tSize.x;
	normCoords.y0 = 1.0f - ((float)params.y/(float)tSize.y);
	normCoords.x1 = (float)normCoords.x0 + ((float)params.w/(float)tSize.x);
	normCoords.y1 = (float)normCoords.y0 - ((float)params.h/(float)tSize.y);

	struct { float u, v; } texcoordsArray[4] = {
	{ normCoords.x0, normCoords.y0 },
	{ normCoords.x0, normCoords.y1 },
	{ normCoords.x1, normCoords.y1 },
	{ normCoords.x1, normCoords.y0 }
};

	float h = static_cast<float>(abs(params.h));
	float w = static_cast<float>(abs(params.w));
	struct { float x, y; } vertcoordsArray[4] = {
	{0, 0},{0,-h},{w,-h},{w, 0}
};

	unsigned short indices[] = { 3, 0, 1, 1, 2, 3 };

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	preDrawing();

	math::mat4d model = math::mat4d::fromIdentity();
	model.translate(m_pos.x, m_pos.y, 0.0);
	model.scale(m_unitsPerPixel*m_scale.x, m_unitsPerPixel*m_scale.y, 0.0);
	model.rotate(m_rotate, 0.0, 0.0, 1.0);
	model.translate(static_cast<double>(-params.cx), static_cast<double>(+params.cy), 0);

	glMultMatrixd(model.v);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer  (2, GL_FLOAT, 0, vertcoordsArray);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoordsArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

	postDrawing();
}

void Sprite::preDrawing() {}
void Sprite::postDrawing(){}
