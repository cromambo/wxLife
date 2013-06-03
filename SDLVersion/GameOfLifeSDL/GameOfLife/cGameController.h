#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "cBoard.h"
#include <string>
#include "cScreen.h"
#include "cGameController.h"
#include "cButton.h"
#include <sstream>
#include "cTimer.h"

class cGameController
{
	cBoard field;
	cScreen displayScr;
	SDL_Surface * background;

	SDL_Surface * button_surface;
	bool play;
	bool mousepaused;
	int generation;
	TTF_Font * display_font;
	SDL_Surface * generation_display;	
	SDL_Surface * fps_display;
	int fps_target;
	bool limit_fps;
	int frame_count;
	SDL_Surface * time_display;
	SDL_Color textColor;
	cButton button_1;
	cTimer gameTimer;
	cTimer frame_timer; //to track how long since the last frame update, used to match a target fps
	cTimer fps_second_timer; //for use measuring a second and using the number of frames rendered in the last second to find fps

	SDL_Event event;
	void freeSurfaces();
	void drawFrame();
	void drawField();
	bool handleEvents();
	void loadResources();
	void formatFPS(int);
	void formatTime();
	void calcFPS();
	void capFrameRate();

public:
	void incrementFPS();
	void decrementFPS();
	SDL_Surface * load_image(std::string filename);
	void run();
	cGameController();
	~cGameController();
};

