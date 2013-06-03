#include "cBoard.h"

cBoard::cBoard()
{
	setBoardSize(60, 35);
	clearBoard();
	organism = 0;
	board = 0;
	organism_clip[ALIVE].x = 0;
	organism_clip[ALIVE].y = 0;
	organism_clip[ALIVE].h = 10;
	organism_clip[ALIVE].w = 10;
	organism_clip[DEAD].x = 10;
	organism_clip[DEAD].y = 0;
	organism_clip[DEAD].h = 10;
	organism_clip[DEAD].w = 10;
	y_pos = 25;
	x_pos = 25;
}

cBoard::~cBoard()
{
	SDL_FreeSurface(organism);
	SDL_FreeSurface(board);
}
cBoard::cBoard(int width_in, int height_in)
{
	width = width_in;
	height = height_in;

	clearBoard();
	//field = new [][];

}
int cBoard::getXPos()
{
	return x_pos;
}
int cBoard::getYPos()
{
	return y_pos;
}
void cBoard::setCell(int x, int y, bool alive)
{
	field[x][y].living = alive;
	countBoard();
	drawFieldtoBoard();
}
cell cBoard::getCell(int x, int y)
{
	return field[x][y];
}

void cBoard::loadResources()
{
	organism = loadImage ("organismSprite.png");
	board = SDL_CreateRGBSurface(SDL_SWSURFACE, width*10, height*10, 32,0,0,0,0);
	SDL_FillRect( board, &board->clip_rect, SDL_MapRGB( board->format, 0x00, 0x99, 0x99 ) );
}
void cBoard::drawFieldtoBoard()
{
	for (int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			if(field[i][j].living)
				Draw(organism, board, i*10, j*10, &organism_clip[ALIVE]);	
			else
				Draw(organism, board, i*10, j*10, &organism_clip[DEAD]);
		}
	}
}
SDL_Surface * cBoard::getBoard()
{
	return board;
}
void cBoard::handleEvents(const SDL_Event * event)
{
	int x=0, y=0;

	if((event->type == SDL_MOUSEBUTTONDOWN) && (event->button.button == SDL_BUTTON_LEFT)) //left click
	{
		x = (event->button.x - x_pos) / 10;
		y = (event->button.y - y_pos) / 10;

		if(((x >= 0) && (x <  width ) && ((y >= 0) && (y < height )))) //if within the boundaries of the board
		{
			field[x][y].living = !(field[x][y].living);
			drawFieldtoBoard();
		}
	}
	if((event->type == SDL_MOUSEMOTION) && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))) //moving and left button held down
	{
		x = (event->motion.x - x_pos) / 10;
		y = (event->motion.y - y_pos) / 10;

		if(((x >= 0) && (x <  width ) && ((y >= 0) && (y < height )))) //if within the boundaries of the board
		{
			field[x][y].living = true;
			drawFieldtoBoard();
		}
	}
}

void cBoard::countBoard()
{
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{		
			field[i][j].adjacent = countAdjacentBounded(i,j);
		}
	}
	//	for(std::vector<std::vector <cell> >::iterator itw = field.begin(); itw != field.end(); ++itw)
	//{

	//	for(std::vector<cell>::iterator ith = itw->begin(); ith != itw->end(); ++ith)
	//	{
	//		ith->living = false;
	//		ith->adjacent = 0;
	//	}
	//}

}
bool cBoard::evolve()
{
	//cell temp_field[width][height];

	//go through the field and count all the ajacent neighbors, put in a temp field so data doesnt get changed before its checked
	//alternatively, resave adjacency data then go through after the count to set living/dead, cost another for loop, benefit no temp array, cost less flexible
	countBoard();

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{		
			//int temp_ajacent = field[i][j].adjacent(); //question, will a compiler make an intermediate field for field[i][j].adjacent instead of accessing it 5 times, if i didnt do this?
			//field[i][j].adjacent = temp_ajacent;
			//if (temp_ajacent < 2  && field[i][j].living) //Any live cell with fewer than two live neighbours dies, as if caused by under-population.
			//	field[i][j].living = false;
			//else if (temp_ajacent == 3) //Any live cell with three live neighbours lives on to the next generation. AND Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
			//	field[i][j].living = true;
			//else if (temp_ajacent > 3 &&  field[i][j].living) //Any live cell with more than three live neighbours dies, as if by overcrowding.
			//	field[i][j].living = false;
			//else if (temp_ajacent == 2 && field[i][j].living == true) //Any live cell with two live neighbours lives on to the next generation.
			//	field[i][j].living = true;
			////else 0 dead, 1 dead, 2 dead, >3 dead, stays dead
			switch (field[i][j].adjacent)
			{
			case 0: 
				field[i][j].living = false; //underpopulation
				break;
			case 1: 
				field[i][j].living = false; //underpopulation
				break;
			case 2: 
				if(field[i][j].living == true) 
					field[i][j].living = true; //lives on, but doesnt create new life
				break;
			case 3: 
				field[i][j].living = true; //living on OR reproduction
				break;
			case 4: 
				field[i][j].living = false; //overcrowding
				break;
			case 5: 
				field[i][j].living = false; //overcrowding
				break;
			case 6: 
				field[i][j].living = false; //overcrowding
				break;
			case 7: 
				field[i][j].living = false; //overcrowding
				break;
			case 8: 
				field[i][j].living = false; //overcrowding
				break;
			default:
				field[i][j].living = false;
				break;
			}
		}
	}
	drawFieldtoBoard();
	return true;
}
void cBoard::setBoardSize(int width_in, int height_in)
{
	width = width_in;
	height = height_in;
	//for_each(field.begin(), field.end(), field.clear());
	//for(unsigned int i = 0; i < field.size(); i++)
	//{
	//	field[i].clear();
	//}
	field.resize(width);
	for(std::vector<std::vector <cell> >::iterator it = field.begin(); it != field.end(); ++it)
	{
		it->resize(height);
	}
	//for(::std::vector<cell>::iterator;;)

	//field.clear();
	//field.resize(width);
	//for(unsigned int i = 0; i < height; ++i)
	//{
	//	field[i].clear();
	//}

}
int cBoard::countAdjacentBounded(int w, int h)
{
	//1 a 2
	//b x c
	//3 d 4
	//w + 1 : 1, b, 3, a, x, d		can check to the right
	//w		: b, x, c				can check above and below
	//w - 1 : a, x, d, 2, c 4		can check to the left
	//h + 1 : 1, b, a, x, 2, c		can check below
	//h		: a, x, d				can check right and left
	//h - 1 : b, 3, x, d, c, 4		can check above

	int total = 0;
	int left = w - 1;
	int right = w + 1;
	int above = h - 1;
	int below = h + 1;
	if (left >= 0) //check the three to the left, if in array bounds
	{
		if (field[left][h].living)  //directly left
			total++;
		if (below < height) //if below isnt out of bounds
		{
			if (field[left][below].living) //left and below
				total++;
		}
		if (above >= 0) //if above isnt out of bounds
		{
			if (field[left][above].living) //left and above
				total++;
		}
	}

	if (right < width) //check the three to the right, if in array bounds
	{
		if (field[right][h].living) //directly right
			total++;

		if (below < height) //if below isnt out of bounds
		{
			if (field[right][below].living) //right and down
				total++;
		}
		if (above >= 0) //if above isnt out of bounds
		{		
			if (field[right][above].living) //right and up
				total++;
		}
	}

	//check directly above
	if (above >= 0)
	{
		if (field[w][above].living)
			total++;
	}
	//check directly below
	if ((below) < height)
	{	
		if (field[w][below].living)
			total++;
	}
	return total;
}

void cBoard::clearBoard()
{
	//for(int i = 0; i < width; i++)
	//{
	//	for(int j = 0; j < height; j++)
	//	{
	//		field[i][j].living = false;
	//		field[i][j].adjacent = 0;
	//	}
	//}
	for(std::vector<std::vector <cell> >::iterator itw = field.begin(); itw != field.end(); ++itw)
	{

		for(std::vector<cell>::iterator ith = itw->begin(); ith != itw->end(); ++ith)
		{
			ith->living = false;
			ith->adjacent = 0;
		}
	}

}

void cBoard::fillBoard()
{
	//for(int i = 0; i < width; i++)
	//{
	//	for(int j = 0; j < height; j++)
	//	{
	//		field[i][j].living = true;
	//	}
	//}

	//	for(std::vector<std::vector <cell> >::iterator itw = field.begin(); itw != field.end(); ++itw)
	
	//for(auto itw = field.begin(); itw != field.end(); ++itw) //auto is AWESOME
	//{
	//	for(auto ith = itw->begin(); ith != itw->end(); ++ith)
	//	{
	//		ith->living = true;
	//	}
	//}

	//trying out some stl algorithm and lambda syntax
	std::for_each(field.begin(), field.end(), [] (std::vector<cell>& row)
	{
		row.size();
		std::for_each(row.begin(), row.end(), [] (cell& org)
		{
			org.living = true;
		} );
	} );

	std::vector <int> v;
	v.push_back(1);
	v.push_back(2);

	for ( int& i : v)
	{
		i++;
	}


	countBoard();
}