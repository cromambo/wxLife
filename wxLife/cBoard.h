/*
Sam Mancill, April 2013. Use it however you want.
*/
#pragma once
#include "wx/wx.h"
#include <vector>
#include <algorithm> //for_each
#include <functional> //for std::function, for passing lamdba functions to traversal functions
#include <array> //std::tr1::array, didnt want standard arrays cause no bounds checking, vectors suck with initialization lists
#include "boost/multi_array.hpp"
//#include "lifeView.h"
//#include "lifeControl.h"
#ifdef __WXMSW__ //this shows any memory leaks on exit
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif 

//living is primary characteristic of a cell/organism/element
//adjacent is how many cells around the current are also living, min 0, max 8, think of a 3x3 grid with this cell at center
//changed is provided to limit the amount of processing done when drawing the board, only changed things need to be drawn
struct cell { bool living; int adjacent; bool changed; };

#define PATTERN_GLIDER_DOWN_RIGHT {false, true, false, false, true, true, true, true, false}
#define PATTERN_GLIDER_DOWN_LEFT {false,true,false,true,false,true,true,true,false}
#define PATTERN_GLIDER_UP_RIGHT {true,true,true,false,true,false,true,false,false}
#define PATTERN_GLIDER_UP_LEFT {true,true,true,true,false,false,true,false,false}
#define PATTERN_SQUARE {true,true,true,true,true,true,true,true,true}
#define PATTERN_EMPTY {false,false,false,false,false,false,false,false,false}

//Notation: for example conways rule is B3/S23, which means a cell is Born if it has 3 living cells around it
//and if alive, Survives if it has 2 or 3 living cells around it. This is implemented with arrays for what a
//cell does if alive (aka CONWAY_SURVIVE), and the number of living cells around it is used as the index into
//the array, and the value at that index is the expected behaviour. The SURVIVE array is used for living cells.
//The BIRTH array is used for dead cells.
//Example: with conways rule, a cell is alive and has 6 living neighbors. CONWAY_SURVIVE[6] returns false, so that cell becomes dead.
//Example: with conways rule, a cell is dead and has 3 living neighbors. CONWAY_BIRTH[3] returns true, so that cell becomes alive.
//	_("Conways(B3/S23)"), 
static bool CONWAY_SURVIVE[] = {false, false, true, true, false, false, false, false, false}; //s23
static bool CONWAY_BIRTH[] = {false, false, false, true, false, false, false, false, false}; //b3
//_("MazeFill(B3/S12345)"), 
static bool  MAZE_SURVIVE[] = {false, true, true, true, true, true, false, false, false}; //s12345
static bool  MAZE_BIRTH[] = {false, false, false, true, false, false, false, false, false}; //b3
//_("Move(B368/S245)"), 
static bool MOVE_SURVIVE[] = {false, false, true, false, true, true, false, false, false}; //s245
static bool MOVE_BIRTH[] = {false, false, false, true, false, false, true, false, true}; //b368
//_("Highlife(B36/S23)"),
static bool HIGHLIFE_SURVIVE[] = {false, false, true, true, false, false, false, false, false}; //s23
static bool HIGHLIFE_BIRTH[] = {false, false, false, true, false, false, true, false, false}; //b36
//_("Seeds(B3/S)"), 
static bool SEEDS_SURVIVE[] = {false, false, false, false, false, false, false, false, false}; //s
static bool SEEDS_BIRTH[] = {false, false, false, true, false, false, false, false, false}; //b3
//_("Sierpinski(B1/S12)")};
static bool SIERPINSKI_SURVIVE[] = {true, true, false, false, false, false, false, false, false}; //s12
static bool SIERPINSKI_BIRTH[] = {false, true, false, false, false, false, false, false, false}; //b1
//_("Coral(B3/S45678)"), 
static bool CORAL_SURVIVE[] = {false, false, false, false, true, true, true, true, true}; //s45678
static bool CORAL_BIRTH[] = {false, false, false, true, false, false, false, false, false}; //b3
//_("Assimilation(B345/S4567)"), 
static bool ASSIMILATION_SURVIVE[] = {false, false, false, false, true, true, true, true, false}; //s4567
static bool ASSIMILATION_BIRTH[] = {false, false, false, true, true, true, false, false, false}; //b345
//_("Amoeba(B357/S1358)"), 
static bool AMOEBA_SURVIVE[] = {false, true, false, true, false, true, false, false, true}; //s1358
static bool AMOEBA_BIRTH[] = {false, false, false, true, false, true, false, true, false}; //b357
//_("Coagulation(B367/S125678)"),
static bool COAGULATION_SURVIVE[] = {false, true, true, false, false, true, true, true, true}; //s125678
static bool COAGULATION_BIRTH[] = {false, false, false, true, false, false, true, true, false}; //b367
//_("Ice(B3678/S235678)")
static bool ICE_SURVIVE[] = {false, false, true, true, false, true, true, true, true}; //s235678
static bool ICE_BIRTH[] = {false, false, false, true, false, false, true, true, true}; //b3678

//generic
//static bool NAME[] = {false, false, false, false, false, false, false, false, false}; //s
//static bool NAME[] = {false, false, false, false, false, false, false, false, false}; //b

class lifeView;
class lifeControl; //forward declare to avoid some circular includes

class cBoard :public wxEvtHandler
{
public:
	cBoard(int rows = 60, int columns = 35, bool bounded = true);
	~cBoard();
	//cBoard(const cBoard& board); //default copy constructor is fine, even shallow copy of the notify pointers.

	enum {ID_Time_Evolve};
	int getAdjacent(int row, int col) const;
	int getRows() const;
	int getColumns() const;
	bool getBounded() const;
	void setBounded(bool bounded_in);
	int getChangeCount() const;
	cell getCell(int row, int col) const;
	bool setCell(int row, int col, bool alive);
	bool invertCell(int row, int col);
	int getCellsHandledLastEvolve() const;
	void SetChangedFlags(bool val);
	void SetSpeed(int spd);
	int getSpeed() const;

	void Evolve(unsigned int num = 1);
	void setEvolveRule(int rule_flag = CONWAY);

	enum evolveRules {CONWAY, MAZE, MOVE, HIGHLIFE, SEEDS, SIERPINSKI, CORAL, ASSIMILATION, AMOEBA, COAGULATION, ICE};

	void clearBoard();
	void fillBoard();
	int countAdjacent(int row, int col) const;
	void setBoardSize(int rows, int columns);
	void setWithPattern(int row, int col, std::array<bool, 9> pattern); //a "glider" design, good for seeing how the program works
	bool isValidElement(int row, int col) const;

	void BeginEvolveTimer();
	void setControlNotificationTarget(lifeControl* ptr);
	void setViewNotificationTarget(lifeView* ptr);
private:

	lifeView* viewNotify;
	lifeControl* controlNotify;

	void countBoard();
	bool evolveOnce();

	static const unsigned int size_next_state_vectors = 9;
	std::vector<bool> NextStateIfLiving;
	std::vector<bool> NextStateIfDead;

	wxTimer evolveTimer;
	int evolve_speed; //the frequency of board updates

	//size of the board. Note that rows = height = y and columns = width = x, so dont call functions that take
	// func(row, col) with func(x, y) and expect the right results. I probably should have done this the other way around, sorry.
	int m_rows, m_columns;

	//if true, outside the edges of board count as dead cells. if false, the board is "wraparound", left
	//and right sides are contiguous, as are top and bottom. Upper left and lower right are considered diagonal,
	//as are upper right and lower left
	bool m_bounded; 
	int cells_handled_last_evolve;

	//represents a two dimensional array of cells, outside vector of rows, containing columns. accessed [row][col]
	//std::vector< std::vector < cell>> field;
	boost::multi_array<cell, 2> field;

	/*Helper function, to be called only from evolve().
	Wanted this to be as fast as possible because it is called a lot when processing the board, so
	it doesn't error check index because evolve function guarantees correct bounds with for loops. 
	Sets alive/dead state. 
	Doesn't tells neighbors this changed, because that is handled at the end of evolve.
	Doesnt update neighbor's count of adjacent living cells, because in the evolve function, 
	the whole board needs to be considered checked at the same time. If processing one spot were to 
	change a spot that hasnt been handled yet, that unhandled spot could do sometime different when 
	the time comes to handle it. 
	Note that at the end of the evolve function, the whole board will be counted to find 
	all the correct counts of nearby living cells and flag all changed cells.*/	
	void setCellQuick(int row, int col, bool alive);

	//adds int value to adjacency count of all nearby cells, use with 1 or -1 to increment or decrement
	void AdjustAdjacent(int row, int col, int val);

	//the const here is to support const objects.
	//The passed function CAN change stuff, if called from a non const function.
	//Other const methods, like countAdjacent, cannot pass functions which would change the object.
	void TraverseAdjacent(int row, int col, std::function<void (int row, int col)> func) const;
	void TraverseAdjacentBounded(int row, int col, std::function<void (int row, int col)> func) const;
	void TraverseAdjacentUnBounded(int row, int col, std::function<void (int row, int col)> func) const;

	//i cant get this to support const and non const :(
	void TraverseAllCells(std::function<void (cell& c)> func);
	void TraverseAllIndices(std::function<void (int row, int col)> func) const;

	void OnEvolveTimer(wxTimerEvent& evt);
};
