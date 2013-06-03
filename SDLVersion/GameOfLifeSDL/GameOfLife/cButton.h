#pragma once
#include "SDL.h"

enum  { CLIP_MOUSEOVER = 0, CLIP_MOUSEOUT = 1, CLIP_MOUSEDOWN = 2, CLIP_MOUSEUP = 3}; //

class cButton
{
private:
	//The attributes of the button
	SDL_Rect button_dim;

	//The part of the button sprite sheet that will be shown
	SDL_Rect button_clips[4];
	SDL_Rect * clip;
public:
	//Initialize the variables
	cButton( int x, int y, int w, int h );
	~cButton();

	//Handles events and set the button's sprite region
	void handleEvents(const SDL_Event *);

	//Shows the button on the screen
	SDL_Rect* getClip();
	SDL_Rect getDim();
	void set_clips();
};
