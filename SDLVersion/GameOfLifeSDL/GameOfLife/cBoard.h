#pragma once
#include "cGuiObject.h"
#include <vector>
#include <algorithm> //for_each

struct cell { bool living; int adjacent; };

enum cellState {ALIVE, DEAD};

class cBoard :public cGuiObject {
	int width, height;
	//cell field[WIDTH][HEIGHT]; //todo: switch this over to a vector, make it dynamic
	std::vector< std::vector < cell>> field;
	SDL_Surface * organism;	
	SDL_Surface * board;
	int y_pos; //the x and y pixel of position of the board object on the screen
	int x_pos;
	SDL_Rect organism_clip[2];
	void drawFieldtoBoard();
public:
	cBoard();
	cBoard(int width, int height);
	~cBoard();

	//display functions
	int getXPos();
	int getYPos();
	void handleEvents(const SDL_Event * event);
	void loadResources();
	SDL_Surface * getBoard();

	//logic functions
	void setCell(int x, int y, bool alive);
	cell getCell(int x, int y);
	void countBoard();
	bool evolve();
	void clearBoard();
	void fillBoard();
	int countAdjacentBounded(int i, int j);
	void setBoardSize(int width, int height);



};
