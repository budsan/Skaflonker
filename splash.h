#ifndef SPLASH_H
#define SPLASH_H

#include "standardsprite.h"
#include "graphics/camera2d.h"
#include "graphics/tools/texthud.h"
#include "input/keyboardlistener.h"

class Skaflonker;

class Splash : public Guy::KeyboardListener
{
public:
	Splash(Skaflonker &parent);

	void   load();
	void unload();

	void update(double deltaTime);
	void draw();

protected:
	void onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods) override;
	void onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods) override;

private:
	Skaflonker &parent;

	double totalTime;
	double fadeTime;
	Guy::Camera2D camera;
	Guy::TextHUD pressAny;
	StandardSprite splash;
};

#endif // SPLASH_H
