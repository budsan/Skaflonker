#ifndef BOX_H
#define BOX_H

#include "animatedsprite.h"
#include <math/algebra3.h>

class Box : public AnimatedSprite
{
public:
	Box(const math::vec3d &position = math::vec3d());

	void update() override;
};

#endif // BOX_H
