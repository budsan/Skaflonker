#include "ingame.h"

#include "environment.h"
#include <set>
#include <time.h>

const std::vector<std::string> Backgrounds{/*"background-1.png",*/ "background-2.png", "background-3.png"};
const std::vector<std::string> PlayerLibraries{"data/animations/dinoazul", "data/animations/dinorojo", "data/animations/dinoverde",
											  "data/animations/huevoazul", "data/animations/huevovermell", "data/animations/huevoverd"};

Ingame::Ingame() : player(0), player2(1), m_currentBackground(0)
{
	m_shadowSprite.setFileName("shadow.png");
	m_backgroundSprite.setScale(6.0);
	nextBackground();
	srand(time(nullptr));
}

void Ingame::update(double deltaTime)
{
	m_accumulatedTime += deltaTime;
	player.update(deltaTime);
	player2.update(deltaTime);
	if (m_accumulatedTime > 5.0) {
		m_accumulatedTime = 0;
		nextBackground();
		player.setLibrary(randomLibrary());
		player2.setLibrary(randomLibrary());
	}
}

void Ingame::load()
{
	player.setLibrary(randomLibrary());
	player.playTrack("idle");

	player2.setLibrary(randomLibrary());
	player2.playTrack("idle");

	camera.init();
	camera.setPos(math::vec2f(0, 600));
}

void Ingame::draw()
{
	glDisable(GL_DEPTH_TEST);

	Guy::Screen &screen = Guy::Environment::instance().screen();
	screen.fillWithColor(Guy::rgba(0.75f, 0.75f, 0.75f, 1));

	math::vec2d p1ProjPos = player.projectedPosition();
	math::vec2d p2ProjPos = player2.projectedPosition();

	double dist = (p2ProjPos - p1ProjPos).module();
	math::vec2d cent = (p1ProjPos + p2ProjPos)/2;

	cameraPos = cameraPos * 0.9 + cent * 0.1;
	camera.setPos(cameraPos + math::vec2d(0, 300));

	if (dist < 1200) dist = 1200;
	float currentZoom = 600/dist;
	zoom = zoom * 0.75 + currentZoom * 0.25;
	camera.setZoom(zoom);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.projectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.viewMatrix().v);

	m_backgroundSprite.draw();

	m_shadowSprite.setPosition(player.floorPosition());
	m_shadowSprite.draw();

	m_shadowSprite.setPosition(player2.floorPosition());
	m_shadowSprite.draw();

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

void Ingame::nextBackground()
{
	m_currentBackground = (++m_currentBackground) % Backgrounds.size();
	m_backgroundSprite.setFileName(Backgrounds.at(m_currentBackground));
}

std::shared_ptr<Player::Library> Ingame::randomLibrary() const
{
	return Player::loadDirectory(PlayerLibraries.at(std::rand() % PlayerLibraries.size()));
}
