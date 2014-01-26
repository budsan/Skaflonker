#include "splash.h"

#include "skaflonker.h"
#include "graphics/texturemanager.h"

Splash::Splash(Skaflonker &parent) : parent(parent), fadeTime(0.0)
{
}

void Splash::load()
{
	pressAny.loadFont("data/font/cutecartoon.ttf", 50);
	pressAny.font()->setAlignment(Guy::Font::CENTER);
	pressAny.setColor(Guy::rgba(0.2,0.2,0.2,1));
	pressAny.clampedPos() = math::vec2f(0.45f, 0.4f);

	splash.setFileName("splash.png");
	camera.init();
}

void Splash::unload()
{

}

void Splash::update(double deltaTime)
{
	totalTime += deltaTime;

	if (fadeTime > 0.0) {
		fadeTime -= deltaTime;
		if (fadeTime < 0.0) {
			parent.setState(Skaflonker::GameState);
		}
	}
}

void Splash::draw()
{
	glDisable(GL_DEPTH_TEST);

	Guy::Screen &screen = Guy::Environment::instance().screen();
	screen.fillWithColor(Guy::rgba(0.75f, 0.75f, 0.75f, 1));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera.projectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera.viewMatrix().v);

	if (fadeTime <= 0) {
		pressAny.displayText() = std::string("Press any button to play");
		pressAny.setColor(Guy::rgba(0.25f, 0.25f, 0.25f, (std::sin(totalTime*4)*0.5) + 0.5));
	}
	else {
		pressAny.displayText() = std::string("Get ready for the battle!");
		pressAny.setColor(Guy::rgba(0.25f, 0.25f, 0.25f, (std::sin(totalTime*32)*0.5) + 0.5));
	}


	splash.draw();
	pressAny.draw();
}

void Splash::onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{

}

void Splash::onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	if (fadeTime <= 0.0) fadeTime = 3.0;
}
