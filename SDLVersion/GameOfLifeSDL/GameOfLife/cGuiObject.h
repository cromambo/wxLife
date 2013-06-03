#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
class cGuiObject
{
protected:
	SDL_Surface * loadImage(std::string filename);
	void Draw(SDL_Surface * src, SDL_Surface * dest, int x, int y, SDL_Rect* clip);
public:
	cGuiObject(void);
	~cGuiObject(void);
	void Draw();
	void Load();
	void onclick();
	void onPress();
};

