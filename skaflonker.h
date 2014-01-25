#pragma once

#include "guyframework/game.h"
#include "guyframework/input/input.h"
#include "guyframework/input/keyboardlistener.h"

#include "guyframework/graphics/camera2d.h"
#include "guyframework/graphics/tools/texthud.h"
#include "guyframework/graphics/tools/frameshud.h"

#include "player.h"


class Skaflonker : public Guy::Game, public Guy::FocusListener, public Guy::KeyboardListener
{
public:
    Skaflonker();
    virtual ~Skaflonker();

protected:
	void init();

	void   load();
	void unload();

	void update(double deltaTime);
	void draw();

	virtual const char *name();
	virtual const char *version();

	void onGainInputFocus();
	void onLoseInputFocus();

	void onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
	void onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);

	Guy::FramesHUD frames;
	Guy::Camera2D camera;

	Player player;
    Player player2;
};

