#pragma once

#include "player.h"
#include "graphics/camera2d.h"

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

	Player player;
	Player player2;
};
