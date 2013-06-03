#include "cTimer.h"


cTimer::cTimer(void)
{
	running = false;
	paused = false;
	startTick = 0;
	pausedTick = 0;
	
}


cTimer::~cTimer(void)
{
}

void cTimer::pause(void)
{
	if(running && !paused)
	{
		running = false;
		paused = true;
		pausedTick += SDL_GetTicks() - startTick;
		startTick = 0;
	}
}

void cTimer::unpause(void)
{
	if(paused)
	{
		paused = false;
		running = true;
		startTick = SDL_GetTicks();
	}
}


void cTimer::start(void)
{
	if(paused)
		unpause();
	if(!running)
	{
		running = true;
		paused = false;
		pausedTick = 0;
		startTick = SDL_GetTicks();
	}
}
void cTimer::restart(void)
{
	running = true;
	paused = false;
	pausedTick = 0;
	startTick = SDL_GetTicks();
}

void cTimer::stop(void)
{
	if(running)
	{
		pausedTick = pausedTick + SDL_GetTicks() - startTick; //end - start, add in any time from before pauses
	}

	running = false;
	paused = false;

}

std::string cTimer::status(void)
{
	if (running)
		return "running";
	else if (paused)
		return "paused";
	else
		return "stopped";

}


int cTimer::getElapsed(void)
{
	if(running)
		return( pausedTick + SDL_GetTicks() - startTick);
	return pausedTick;
}


void cTimer::handleEvents(const SDL_Event * event)
{
	int x=0, y=0;

	switch (event->type) 
	{
	case SDL_KEYDOWN: 
		switch(event->key.keysym.sym)
		{
		case SDLK_p: 			
			pause();
			break;
		case SDLK_u: 			
			unpause();
			break;
		case SDLK_r: 			
			start();
			break;
		case SDLK_s: 			
			stop();
			break;
		default:;
		};
	default: ;
	}


}
