#include "ingame.h"

#include "environment.h"

Ingame::Ingame() : player(0), player2(1)
{
}

void Ingame::update(double deltaTime)
{
	player.update(deltaTime);
	player2.update(deltaTime);
}

void Ingame::load()
{
	std::shared_ptr<Player::Library> dinoazul = Player::loadDirectory("data/animations/dinoazul");
	player.setLibrary(dinoazul);
	player.playTrack("idle");

	std::shared_ptr<Player::Library> dinorojo = Player::loadDirectory("data/animations/dinorojo");
	player2.setLibrary(dinorojo);
	player2.playTrack("idle");

	camera.init();
	camera.setPos(math::vec2f(0, 600));
}

void Ingame::draw()
{
	glDisable(GL_DEPTH_TEST);

	Guy::Screen &screen = Guy::Environment::instance().screen();
	screen.fillWithColor(Guy::rgba(0.75f, 0.75f, 0.75f, 1));

	camera.setZoom(0.5);

	math::vec3d& p1pos = player.position();
	math::vec3d& p2pos = player2.position();
	math::vec2d p1ProjPos(p1pos.x, p1pos.y + (p1pos.z/2));
	math::vec2d p2ProjPos(p2pos.x, p2pos.y + (p2pos.z/2));

	double dist = (p2ProjPos - p1ProjPos).module();
	math::vec2d cent = (p1ProjPos + p2ProjPos)/2;

	cameraPos = cameraPos * 0.95 + cent * 0.05;
	camera.setPos(cameraPos + math::vec2d(0, 300));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.projectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.viewMatrix().v);

	if (player.position().z > player2.position().z )
	{
		player.draw();
		player2.draw();
	}
	else
	{
		player2.draw();
		player.draw();
	}
}
