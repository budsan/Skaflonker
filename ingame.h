#pragma once

#include "player.h"
#include "graphics/camera2d.h"
#include "standardsprite.h"

class Box;
class Skaflonker;

class Ingame
{
public:
	Ingame(Skaflonker& parent);

	void   load();
	void unload();

	void update(double deltaTime);
	void draw();

	void resetGame();

private:
	void nextBackground();
	std::shared_ptr<Player::Library> randomLibrary() const;

	Box *createRandomBox() const;

	Skaflonker &parent;

	Guy::Camera2D camera;
	math::vec2d cameraPos;
	double zoom;

	Player player;
	Player player2;
	StandardSprite m_shadowSprite;
	StandardSprite m_backgroundSprite;

	std::vector<Box *> m_boxes;

	double m_accumulatedTime;
	int m_currentBackground;
};
