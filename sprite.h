#pragma once

#include "math/algebra3.h"

class Sprite
{
public:
	Sprite();
	virtual ~Sprite();

	void draw();

	static void setGlobalPixelsPerUnit(double value);
	void setPixelsPerUnit(double value);

	void setScale(double value);
	void setScale(math::vec2d value);
	void setScaleWidth(double value);
	void setScaleHeight(double value);

	void setRotation(double value);

	math::vec2d scale() { return m_scale; }
	double rotation()  { return m_rotate; }

	const math::vec3d &position() const { return m_pos;}
	void setPosition(const math::vec3d &_position) {m_pos = _position;}
	math::vec3d &position() { return m_pos;}
private:
	static double s_globalUnitsPerPixel;
	double m_unitsPerPixel;

	math::vec2d m_scale;
	double m_rotate;

protected:
	struct DrawParameters {
		const char* filename;
		int x, y, w, h, cx, cy;
	};

	virtual void drawParameters(Sprite::DrawParameters &params) = 0;
	virtual void preDrawing();
	virtual void postDrawing();

	math::vec3d m_pos;
};
