#include "cScreen.h"

cScreen::cScreen(void)
{
	//Start SDL
	//if (
	SDL_Init( SDL_INIT_EVERYTHING ); //returns -1 on error
	init_screen(640, 480, 32, SDL_SWSURFACE);
	TTF_Init(); //returns -1 on error
	//== -1)// if screen setup failed
	//return false;
	fps = 60;
}

cScreen::~cScreen(void)
{
	//Quit SDL
	TTF_Quit();
	SDL_Quit();
}

bool cScreen::init_screen(int height, int width, int bpp, unsigned int flags)
{
	screen = SDL_SetVideoMode(height, width, bpp, flags);
	return (screen != nullptr);
}

void cScreen::Draw(SDL_Surface * surface)
{
	SDL_BlitSurface(surface, nullptr, screen, nullptr);
}
void cScreen::Draw(SDL_Surface * surface, int x, int y, SDL_Rect* clip)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(surface, clip, screen, &offset);
}
void cScreen::Erase()
{
	//Fill the screen white
	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
}
void cScreen::update()
{
	SDL_Flip(screen);
}