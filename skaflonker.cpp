#include "skaflonker.h"

#include "guyframework/environment.h"
#include "guyframework/debug.h"

#include <functional>
#include <sstream>
#include <ctime>

#define GAME_NAME "Skaflonker"
#define GAME_VERSION "1"

Skaflonker theGame;

Skaflonker::Skaflonker()
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
}

void Skaflonker::load()
{

}

void Skaflonker::unload()
{

}

void Skaflonker::update(double deltaTime)
{

}

void Skaflonker::draw()
{

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


