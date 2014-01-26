#include "ingame.h"
#include "box.h"

#include "skaflonker.h"

#include "environment.h"
#include <set>
#include <time.h>
#include <algorithm>
#include "sprite.h"
#include "lavaevent.h"

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

const math::vec2d ViewSize{1920, 1080};
const math::vec2d BackgroundSize{1600, 900};
constexpr double BackgroundScale{6.0};
const math::vec2d BackgroundSizeScaled = BackgroundSize * BackgroundScale;
constexpr std::size_t MaxBoxAmount{10};

Ingame::Ingame(Skaflonker& parent)
	: parent(parent),
	  player(0),
	  player2(1),
	  m_currentBackground(0),
	  m_environmentEvent(nullptr)
{
	m_shadowSprite.setFileName("shadow.png");
	m_backgroundSprite.setScale(BackgroundScale);
	nextBackground();
	srand(time(nullptr));
}

void worldCollision(Player& player)
{
	math::vec3d& pos = player.position();
	if (pos.x < -4600) {
		pos.x = -4600;
		player.onCollisionLeft();
	} else if (pos.x > 4600) {
		pos.x = 4600;
		player.onCollisionRight();
	}

	if (pos.z < -5200) {
		pos.z = -5200;
		player.onCollisionBottom();
	} else if (pos.z > 800) {
		pos.z = 800;
		player.onCollisionTop();
	}
}

void Ingame::update(double deltaTime)
{
	m_accumulatedTime += deltaTime;
	player.update();
	player2.update();

	// If players collide with eachother
//	std::cout << player.bodyHitBoxes().size() << ", " << player2.bodyHitBoxes().size() << std::endl;
	if (Player::hitBoxesHit(player.bodyHitBoxes(), player2.bodyHitBoxes())) {
		std::cout << "Colission" << std::endl;
////		math::vec2d center = (player.floorPosition() + player2.floorPosition()) / 2.0;
//		math::vec2d a = player.floorPosition() - player2.floorPosition();
//		a.normalize();
//		a = a * 2000.0;
//		std::cout << "Colission: " << a.x << ", " << a.y << std::endl;
////		player.position() -= math::vec3d(a.x, 0, a.y);
	}

	// If players collide with world boundaries
	worldCollision(player);
	worldCollision(player2);

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

	if (m_environmentEvent && m_environmentEvent->isExpired()) {
		changeToRandomEnvironmentEvent();
	}
}

void Ingame::load()
{
	for (std::size_t i = 0; i < PlayerLibrariesPath.size(); ++i) {
		PlayerLibraries.push_back(Player::loadDirectory(PlayerLibrariesPath[i]));
	}

	const double PlayerSeparation = 1200.0;
	player.setLibrary(PlayerLibraries[0]);
	player.playTrack("idle");
	player2.setFacingDirection(Player::RightDirection);
	player.setPosition(math::vec3d(-PlayerSeparation, 2000.0, 0.0));

	player2.setLibrary(PlayerLibraries[0]);
	player2.playTrack("idle");
	player2.setFacingDirection(Player::LeftDirection);
	player2.setPosition(math::vec3d(PlayerSeparation, 2000.0, 0.0));

	for(int i = 0; i < MaxBoxAmount; ++i) {
		m_boxes.push_back(createRandomBox());
	}

	camera.init();
	camera.resizeScreen(ViewSize.x, ViewSize.y);
	camera.setPos(math::vec2f(0, 600));
}

void Ingame::resetGame()
{
	cameraPos = math::vec2d(0,0);

	player.setPosition(math::vec3d(-500, 0, 0));
	player.setFacingDirection(Player::RightDirection);
	player2.setPosition(math::vec3d(500, 0, 0));
	player2.setFacingDirection(Player::LeftDirection);
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

	// Draw background
	m_backgroundSprite.draw();

	// Draw shadows
	for(Box *box : m_boxes) {
		m_shadowSprite.setPosition(box->floorPosition());
		m_shadowSprite.draw();
	}

	m_shadowSprite.setPosition(player.floorPosition());
	m_shadowSprite.draw();

	m_shadowSprite.setPosition(player2.floorPosition());
	m_shadowSprite.draw();

	// Draw ordering by Z
	std::vector<Sprite *> sprites;
	for(Box *box : m_boxes) {
		sprites.push_back(box);
	}
	sprites.push_back(&player);
	sprites.push_back(&player2);

	std::sort(sprites.begin(), sprites.end(), [](Sprite *a, Sprite *b) {
		return a->position().z > b->position().z;
	});

	for(Sprite *sprite : sprites) {
		sprite->draw();
	}
}

void Ingame::nextBackground()
{
	m_currentBackground = (++m_currentBackground) % Backgrounds.size();
	m_backgroundSprite.setFileName(Backgrounds.at(m_currentBackground));
}

void Ingame::changeToRandomEnvironmentEvent()
{
	if (m_environmentEvent) {
		delete m_environmentEvent;
	}

	m_environmentEvent = new LavaEvent;
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
