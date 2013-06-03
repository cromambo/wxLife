#pragma once
#include "SDL.h"
#include <string>

class cTimer
{
private:
	bool running, paused;
	int startTick;

	int pausedTick;
public:
	std::string status(void);
	void pause(void);
	void unpause(void);
	void start(void);
	void restart(void);
	void stop(void);
	cTimer(void);
	~cTimer(void);
	int getElapsed(void);
	void handleEvents(const SDL_Event *);
};
