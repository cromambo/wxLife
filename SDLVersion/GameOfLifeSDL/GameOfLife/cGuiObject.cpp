#include "cGuiObject.h"


cGuiObject::cGuiObject(void)
{
}


cGuiObject::~cGuiObject(void)
{
}

SDL_Surface * cGuiObject::loadImage(std::string filename)
{
	SDL_Surface * loaded_image = nullptr;
	SDL_Surface * optimized_image = nullptr;

	loaded_image = IMG_Load(filename.c_str ()); //24 bit bitmap, need to turn it to 32 bit

	if(loaded_image != nullptr)
	{
		optimized_image = SDL_DisplayFormat(loaded_image); // display format matches screen display

		if (optimized_image != nullptr)
		{
			unsigned int colorkey = SDL_MapRGB (optimized_image->format, 0, 0xFF, 0xFF); //transparency color is a teal blue
			SDL_SetColorKey(optimized_image, SDL_SRCCOLORKEY, colorkey); //make all the teal in the image be transparent
		}
		SDL_FreeSurface(loaded_image);
	}
	return optimized_image;
}

void cGuiObject::Draw(SDL_Surface * src, SDL_Surface * dest, int x, int y, SDL_Rect* clip)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(src, clip, dest, &offset);
}

