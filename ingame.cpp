#include "ingame.h"
#include "box.h"

#include "environment.h"
#include <set>
#include <time.h>

const std::vector<std::string> Backgrounds {
//	"background-1.png",
	"background-2.png",
	"background-3.png"
};

const std::vector<std::string> PlayerLibrariesPath {
	"data/animations/dinoazul",
	"data/animations/dinorojo",
	"data/animations/dinoverde",
	"data/animations/huevoazul",
	"data/animations/huevovermell",
	"data/animations/huevoverd"
};

std::vector<std::shared_ptr<Player::Library>> PlayerLibraries;

const math::vec2d ViewSize{1920, 1080};
const math::vec2d BackgroundSize{1600, 900};
constexpr double BackgroundScale{6.0};
const math::vec2d BackgroundSizeScaled = BackgroundSize * BackgroundScale;
constexpr std::size_t MaxBoxAmount{100};

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
	if (pos.x < -4600) {
		pos.x = -4600;
		clamped = true;
	} else if (pos.x > 4600) {
		pos.x = 4600;
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

	for (Box *box : m_boxes) {
		box->update();
	}

	auto it = m_boxes.begin();
	while (it != m_boxes.end()) {
		Box *box = *it;
		if (box->position().y <= 0.1) {
			it = m_boxes.erase(it);
			delete box;
		}
		else {
			++it;
		}
	}

	for (int i = m_boxes.size(); i < MaxBoxAmount; ++i) {
		m_boxes.push_back(createRandomBox());
	}

	if (m_accumulatedTime > 5.0) {
		m_accumulatedTime = 0;
		nextBackground();
		//player.setLibrary(randomLibrary());
		//player2.setLibrary(randomLibrary());
	}
}

void Ingame::load()
{
	for (std::size_t i = 0; i < PlayerLibrariesPath.size(); ++i) {
		PlayerLibraries.push_back(Player::loadDirectory(PlayerLibrariesPath[i]));
	}

	player.setLibrary(PlayerLibraries[0]);
	player.playTrack("idle");

	player2.setLibrary(PlayerLibraries[3]);
	player2.playTrack("idle");

	for(int i = 0; i < MaxBoxAmount; ++i) {
		m_boxes.push_back(createRandomBox());
	}

	camera.init();
	camera.resizeScreen(ViewSize.x, ViewSize.y);
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

	double dist = (p2ProjPos - p1ProjPos).module() * 0.66;
	math::vec2d cent = (p1ProjPos + p2ProjPos)/2;

	cameraPos = cameraPos * 0.9 + cent * 0.1;

	if (dist < ViewSize.y) dist = ViewSize.y;
	float currentZoom = (ViewSize.y * 0.75f)/dist;
	zoom = zoom * 0.75 + currentZoom * 0.25;

	if (zoom < (ViewSize.x/BackgroundSizeScaled.x) )
		zoom = (ViewSize.x/BackgroundSizeScaled.x);

	camera.setZoom(zoom);

	double zoominv = 1.0 / zoom;
	math::vec2d rad{zoominv * ViewSize.x * 0.5f, zoominv * ViewSize.y * 0.5f};
	math::vec2d finalCamPos = cameraPos + math::vec2d(0, 200);

	if ((finalCamPos.x + rad.x) > (BackgroundSizeScaled.x * 0.5)) {
		finalCamPos.x = (BackgroundSizeScaled.x * 0.5) - rad.x;
	} else if ((finalCamPos.x - rad.x) < -(BackgroundSizeScaled.x * 0.5)) {
		finalCamPos.x = rad.x - (BackgroundSizeScaled.x * 0.5);
	}

	if ((finalCamPos.y + rad.y) > (BackgroundSizeScaled.y * 0.5)) {
		finalCamPos.y = (BackgroundSizeScaled.y * 0.5) - rad.y;
	} else if ((finalCamPos.y - rad.y) < -(BackgroundSizeScaled.y * 0.5)) {
		finalCamPos.y = rad.y - (BackgroundSizeScaled.y * 0.5);
	}

	camera.setPos(finalCamPos);

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
	return PlayerLibraries.at(std::rand() % PlayerLibraries.size());
}

Box *Ingame::createRandomBox() const
{
	double width = 1600.0 * BackgroundScale;
	double height = 900.0 * BackgroundScale;
	double randomX = -(width / 2.0) + std::fmod(std::rand(), width);
	double randomY = -(height / 2.0) + std::fmod(std::rand(), height);
	double randomZ = std::fmod(std::rand(), 2000);
	return new Box(math::vec3d(randomX, randomZ, randomY));
}
