#include "cGameController.h"

cGameController::cGameController(void)
	: button_1(100,350,160,120)
{
	displayScr.init_screen(640, 480, 32, SDL_SWSURFACE);
	background = nullptr;

	button_surface = nullptr;
	time_display = nullptr;
	fps_display = nullptr;
	generation_display = nullptr;
	generation = 0;
	textColor.r = 255;
	textColor.g = 0;
	textColor.b = 255;
	display_font = nullptr;
	play = false;
	mousepaused = false;
	fps_target = 10;
	limit_fps = false;
	frame_count = 0;

	SDL_WM_SetCaption("The Game of Life", NULL);
}

cGameController::~cGameController(void)
{
	freeSurfaces();

	TTF_CloseFont(display_font);
}

void cGameController::freeSurfaces(void)
{

	SDL_FreeSurface(fps_display);
	SDL_FreeSurface(button_surface);	
	SDL_FreeSurface(background);
	SDL_FreeSurface(generation_display);
	SDL_FreeSurface(time_display);
}
void cGameController::run(void)
{
	loadResources();
	bool quit = false;
	fps_second_timer.restart();
	frame_timer.restart();

	while(!quit) //every iteration of this is a frame
	{
		//events
		quit = handleEvents();

		//logic
		if(play && !mousepaused)
		{
			field.evolve(); //currently this is tying it to the framerate which is not good, so field has to have its own timer and not advance unless the time is right
		}
		frame_count++;

		//rendering
		drawFrame(); //draw everything
		capFrameRate();
	}
}
void cGameController::calcFPS()
{
	if( fps_second_timer.getElapsed() > 1000) //if time since last fps report is more than one second
	{
		formatFPS(frame_count); //create the fps text
		frame_count = 0;
		fps_second_timer.restart();
	}
}
void cGameController::capFrameRate()
{
	if(!limit_fps)
	{
		while (frame_timer.getElapsed() < (1000/fps_target)){} //wait until time for next frame
		frame_timer.restart();
	}
}
void cGameController::formatTime()
{
	std::stringstream time_stream;
	time_stream << "Time(" << gameTimer.status() << "): " << gameTimer.getElapsed() << " ms.";
	time_display = TTF_RenderText_Solid(display_font, time_stream.str().c_str(), textColor);
}
void cGameController::formatFPS(int fps)
{
	std::stringstream fps_stream;
	fps_stream << "FPS: " << fps;
	fps_display = TTF_RenderText_Solid(display_font, fps_stream.str().c_str(), textColor);

}
void cGameController::drawFrame()
{
	displayScr.Erase();
	formatTime(); //create the timer text with current values
	calcFPS();
	displayScr.Draw(background);
	displayScr.Draw(generation_display, 100, 100);
	displayScr.Draw(fps_display, 100, 200);
	displayScr.Draw(time_display, 100, 300);
	displayScr.Draw(button_surface, button_1.getDim().x, button_1.getDim().y, button_1.getClip());
	displayScr.Draw(field.getBoard(), field.getXPos(), field.getYPos());
	displayScr.update();
}

void cGameController::loadResources()
{
	freeSurfaces();


	button_surface = load_image ("buttonSprite.png");
	background = load_image ("background.png");
	display_font = TTF_OpenFont("lazy.ttf", 28);
	generation_display = TTF_RenderText_Solid(display_font, "generation",textColor );
	time_display = TTF_RenderText_Solid(display_font, "timer", textColor);
	fps_display = TTF_RenderText_Solid(display_font, "fps", textColor);
	field.loadResources();
}
bool cGameController::handleEvents()
{
	while ( SDL_PollEvent (&event) )
	{
		button_1.handleEvents(&event); //pass events to all objects that need them
		gameTimer.handleEvents(&event);
		field.handleEvents(&event);
		switch (event.type) 
		{
		case SDL_QUIT : return true;
			break;
		case SDL_KEYDOWN: 
			switch(event.key.keysym.sym)
			{
			case SDLK_l: //thats an L aka elle, font kinda looks like a one
				limit_fps = !limit_fps;
				break;
			case SDLK_ESCAPE: 
				return true;
				break;
			case SDLK_EQUALS:
				incrementFPS();
				break;
			case SDLK_MINUS: 				
				decrementFPS();
				break;
			case SDLK_SPACE: 				
				play = !play;
				if(play)
					mousepaused = false;
				break;
			default:;
			};
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				mousepaused = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				mousepaused = false;
			}
			break;
		default: ;
		}
	}
	return false;
}

void cGameController::incrementFPS()
{
	if (fps_target < 240)
		fps_target += 10;
	else
		fps_target = 250;
}
void cGameController::decrementFPS()
{
	if (fps_target > 10)
		fps_target -= 10;
	else
		fps_target = 2;
}
SDL_Surface * cGameController::load_image(std::string filename)
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
