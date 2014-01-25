#pragma once

#include "player.h"
#include "graphics/camera2d.h"
#include "standardsprite.h"

class Ingame
{
public:
	Ingame();

	void   load();
	void unload();

	void update(double deltaTime);
	void draw();

private:
	Guy::Camera2D camera;
	math::vec2d cameraPos;

	Player player;
	Player player2;
	StandardSprite m_shadowSprite;
};
