#include "ingame.h"
#include "box.h"

#include "environment.h"
#include <set>
#include <time.h>

const std::vector<std::string> Backgrounds{/*"background-1.png",*/ "background-2.png", "background-3.png"};
const std::vector<std::string> PlayerLibraries{"data/animations/dinoazul", "data/animations/dinorojo", "data/animations/dinoverde",
											  "data/animations/huevoazul", "data/animations/huevovermell", "data/animations/huevoverd"};
const double BackgroundScale{6.0};

Ingame::Ingame() : player(0), player2(1), m_currentBackground(0)
{
	m_shadowSprite.setFileName("shadow.png");
	m_backgroundSprite.setScale(BackgroundScale);
	nextBackground();
	srand(time(nullptr));
}
bool clampWorld(math::vec3d& pos)
{
	bool clamped = false;
	if (pos.x < -4500) {
		pos.x = -4500;
		clamped = true;
	} else if (pos.x > 4500) {
		pos.x = 4500;
		clamped = true;
	}

	if (pos.z < -5200) {
		pos.z = -5200;
		clamped = true;
	} else if (pos.z > 800) {
		pos.z = 800;
		clamped = true;
	}

	return clamped;
}

void Ingame::update(double deltaTime)
{
	m_accumulatedTime += deltaTime;
	player.update();
	player2.update();

	if (clampWorld(player.position()))
	{
		//STAPH
	}

	if (clampWorld(player2.position()))
	{
		//STAPH
	}

	for(Box *box : m_boxes) {
		box->update();
	}

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

	for(int i = 0; i < 100; ++i) {
		double width = 1600.0 * BackgroundScale;
		double height = 900.0 * BackgroundScale;
		double randomX = -(width / 2.0) + std::fmod(std::rand(), width);
		double randomY = -(height / 2.0) + std::fmod(std::rand(), height);
		double randomZ = std::fmod(std::rand(), 2000);
		m_boxes.push_back(new Box(math::vec3d(randomX, randomZ, randomY)));
	}

	camera.init();
	camera.resizeScreen(1920, 1080);
	camera.setPos(math::vec2f(0, 600));
}

void Ingame::draw()
{
	glDisable(GL_DEPTH_TEST);

	Guy::Screen &screen = Guy::Environment::instance().screen();
	screen.fillWithColor(Guy::rgba(0.75f, 0.75f, 0.75f, 1));

	// Camera calculations
	math::vec2d p1ProjPos = player.projectedPosition();
	math::vec2d p2ProjPos = player2.projectedPosition();

	double dist = (p2ProjPos - p1ProjPos).module();
	math::vec2d cent = (p1ProjPos + p2ProjPos)/2;

	cameraPos = cameraPos * 0.9 + cent * 0.1;

	if (dist < 1200) dist = 1200;
	float currentZoom = 900/dist;
	zoom = zoom * 0.75 + currentZoom * 0.25;

	if (zoom < (1920.0f/9600.0f) )
		zoom = (1920.0f/9600.0f);

	camera.setZoom(zoom);

	float zoominv = 1.0f / zoom;
	float zoomw = zoominv * 1920.0f * 0.5f;
	float zoomh = zoominv * 1080.0f * 0.5f;
	math::vec2d realCameraPos = cameraPos + math::vec2d(0, 200);

	if ((realCameraPos.x + zoomw) > 4800) {
		realCameraPos.x = 4800 - zoomw;
	} else if ((realCameraPos.x - zoomw) < -4800) {
		realCameraPos.x = zoomw - 4800;
	}

	if ((realCameraPos.y + zoomh) > 2700) {
		realCameraPos.y = 2700 - zoomh;
	} else if ((realCameraPos.y - zoomh) < -2700) {
		realCameraPos.y = zoomh - 2700;
	}

	camera.setPos(realCameraPos);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.projectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.viewMatrix().v);

	m_backgroundSprite.draw();

	for(Box *box : m_boxes) {
		m_shadowSprite.setPosition(box->floorPosition());
		m_shadowSprite.draw();
	}

	m_shadowSprite.setPosition(player.floorPosition());
	m_shadowSprite.draw();

	m_shadowSprite.setPosition(player2.floorPosition());
	m_shadowSprite.draw();

	for(Box *box : m_boxes) {
		box->draw();
	}

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
