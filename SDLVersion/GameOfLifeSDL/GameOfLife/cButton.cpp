#include "cButton.h"

cButton::cButton( int x, int y, int w, int h )
{
	button_dim.x = x;
	button_dim.y = y;
	button_dim.w = w;
	button_dim.h = h;
	set_clips();	
	clip = &button_clips[CLIP_MOUSEOUT];
}


cButton::~cButton(void)
{
}

void cButton::handleEvents(const SDL_Event * event)
{
	int x=0, y=0;

	if(event->type == SDL_MOUSEMOTION)
	{
		x = event->motion.x;
		y = event->motion.y;

		if(((x >= button_dim.x) && (x <= button_dim.x + button_dim.w ) && ((y >= button_dim.y) && (y <= button_dim.y + button_dim.h ))))
		{
			clip = &button_clips[CLIP_MOUSEOVER]; 
		}
		else
		{
			clip = &button_clips[CLIP_MOUSEOUT];
		}
	}
}
SDL_Rect* cButton::getClip()
{
	return clip;
}
SDL_Rect cButton::getDim()
{
	return button_dim;
}
void cButton::set_clips()
{
	//Clip the sprite sheet
	button_clips[ CLIP_MOUSEOVER ].x = 0;
	button_clips[ CLIP_MOUSEOVER ].y = 0;
	button_clips[ CLIP_MOUSEOVER ].w = 160;
	button_clips[ CLIP_MOUSEOVER ].h = 120;

	button_clips[ CLIP_MOUSEOUT ].x = 160;
	button_clips[ CLIP_MOUSEOUT ].y = 0;
	button_clips[ CLIP_MOUSEOUT ].w = 160;
	button_clips[ CLIP_MOUSEOUT ].h = 120;

	button_clips[ CLIP_MOUSEDOWN ].x = 0;
	button_clips[ CLIP_MOUSEDOWN ].y = 120;
	button_clips[ CLIP_MOUSEDOWN ].w = 160;
	button_clips[ CLIP_MOUSEDOWN ].h = 120;

	button_clips[ CLIP_MOUSEUP ].x = 160;
	button_clips[ CLIP_MOUSEUP ].y = 120;
	button_clips[ CLIP_MOUSEUP ].w = 160;
	button_clips[ CLIP_MOUSEUP ].h = 120;    
}