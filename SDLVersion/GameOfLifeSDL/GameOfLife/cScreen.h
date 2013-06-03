#pragma once
#include "SDL.h"

#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include "cTimer.h"

class cScreen
{
	SDL_Surface * screen;
	int fps;
	cTimer fps_time;
public:
	bool init_screen(int height, int width, int bpp, unsigned int flags);

	void Draw(SDL_Surface * );

	void Draw(SDL_Surface * surface, int x, int y, SDL_Rect* clip = nullptr);
	void Erase();
	void update();
	cScreen(void);
	~cScreen(void);
};

