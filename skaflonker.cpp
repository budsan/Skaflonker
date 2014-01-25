#include "skaflonker.h"

#include "guyframework/environment.h"
#include "guyframework/debug.h"

#include "action.h"
#include "actionsfighter.h"

#include <functional>
#include <sstream>
#include <ctime>

#define GAME_NAME "Skaflonker"
#define GAME_VERSION "1"

Skaflonker theGame;

Skaflonker::Skaflonker()
    : player2(1)
{

}

Skaflonker::~Skaflonker()
{

}

const char *Skaflonker::name()
{
	return GAME_NAME;
}

const char *Skaflonker::version()
{
	return GAME_VERSION;
}

void Skaflonker::init()
{
    Guy::Environment &env = Guy::Environment::instance();
    env.setFrameRate(60, Guy::Environment::StableWithFrameSkipDeltaTime);
    env.screen().setMode(Guy::Screen::Mode(0, 0), false);

    Guy::Input &in = env.input();
    in.addFocusListener(this);

    ActionsFighter* actionsFighterOne = new ActionsFighterOne();
    in.keyboard().addListener(actionsFighterOne);

    ActionsFighter* actionsFighterTwo = new ActionsFighterTwo();
    in.keyboard().addListener(actionsFighterTwo);

    std::vector<Actions*> &actions = Actions::instance();
    actions.push_back(actionsFighterOne);
    actions.push_back(actionsFighterTwo);
}

void Skaflonker::load()
{
	if (!frames.loadFont("data/font/cutecartoon.ttf"))
	{
		Guy::printLog("ERROR: Loading frames font\n");
	}

	frames.setColor(Guy::rgba(0,0,0,1));

    std::shared_ptr<Player::Library> dinoazul = Player::loadDirectory("data/animations/dinoazul");
	player.setLibrary(dinoazul);
	player.playTrack("idle");

    std::shared_ptr<Player::Library> dinorojo = Player::loadDirectory("data/animations/dinorojo");
    player2.setLibrary(dinorojo);
    player2.playTrack("idle");

	camera.init();
}

void Skaflonker::unload()
{

}

void Skaflonker::update(double deltaTime)
{
	(void) deltaTime;

    frames.update(deltaTime);
    player.update(deltaTime);
    player2.update(deltaTime);

	Actions::endOfFrameAll();
}

void Skaflonker::draw()
{
	glDisable(GL_DEPTH_TEST);

	Guy::Screen &screen = Guy::Environment::instance().screen();
	screen.fillWithColor(Guy::rgba(0.75f, 0.75f, 0.75f, 1));

	camera.setZoom(0.5);
	camera.setPos(math::vec2f(0, 600));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.projectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.viewMatrix().v);

	player.draw();
    player2.draw();
	frames.draw();
}

void Skaflonker::onGainInputFocus()
{
	Guy::Environment::instance().resume();
}

void Skaflonker::onLoseInputFocus()
{
	Guy::Environment::instance().pause();
}

void Skaflonker::onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	(void) unicode;
	(void) key;
	(void) mods;
}

void Skaflonker::onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	(void) unicode;
	(void) key;
	(void) mods;
}



