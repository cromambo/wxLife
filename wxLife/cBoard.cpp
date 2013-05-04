#include "cBoard.h"
#include "lifeControl.h"

cBoard::cBoard(int rows, int columns, bool bounded)
	:m_bounded(bounded), cells_handled_last_evolve(0), evolve_speed(10), controlNotify(nullptr), viewNotify(nullptr)
{
	setBoardSize(rows, columns);
	clearBoard();
	setEvolveRule(CONWAY);
	evolveTimer.SetOwner(this, ID_Time_Evolve);
	BeginEvolveTimer();
	Bind(wxEVT_TIMER, &cBoard::OnEvolveTimer, this, ID_Time_Evolve);
}

cBoard::~cBoard()
{
}

int cBoard::getAdjacent(int row, int col) const
{
	if(!isValidElement(row, col))
	{
		std::runtime_error("Row or Column out of bounds, in cBoard::getAdjacent.");
		return 0;
	}
	else
		return field[row][col].adjacent;
}
int cBoard::getRows() const
{
	return m_rows;
}
int cBoard::getColumns() const
{
	return m_columns;
}
bool cBoard::getBounded() const
{
	return m_bounded;
}
int cBoard::getChangeCount() const
{
	return cells_handled_last_evolve;
}

void cBoard::setControlNotificationTarget(lifeControl* ptr)
{
	if(ptr)
		controlNotify = ptr;
}
void cBoard::setViewNotificationTarget(lifeView* ptr)
{
	if(ptr)
		viewNotify = ptr;
}
int cBoard::getCellsHandledLastEvolve() const
{
	return cells_handled_last_evolve;
}
void cBoard::setBounded(bool bounded_in)
{
	if(m_bounded != bounded_in) //if it didnt actually change, no need to do a bunch of calculations
	{
		m_bounded = bounded_in;
		countBoard();
	}
}
void cBoard::SetSpeed(int spd)
{
	evolve_speed = spd;
	BeginEvolveTimer();
}
int cBoard::getSpeed() const
{
	return evolve_speed;
}

void cBoard::setEvolveRule(int rule_flag)
{
	switch(rule_flag)
	{
	case CONWAY:
		NextStateIfLiving.assign(CONWAY_SURVIVE, CONWAY_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(CONWAY_BIRTH, CONWAY_BIRTH+size_next_state_vectors);
		break;
	case MAZE:
		NextStateIfLiving.assign(MAZE_SURVIVE, MAZE_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(MAZE_BIRTH, MAZE_BIRTH+size_next_state_vectors);
		break;
	case MOVE:
		NextStateIfLiving.assign(MOVE_SURVIVE, MOVE_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(MOVE_BIRTH, MOVE_BIRTH+size_next_state_vectors);
		break;
	case HIGHLIFE:
		NextStateIfLiving.assign(HIGHLIFE_SURVIVE, HIGHLIFE_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(HIGHLIFE_BIRTH, HIGHLIFE_BIRTH+size_next_state_vectors);
		break;	
	case SEEDS:
		NextStateIfLiving.assign(SEEDS_SURVIVE, SEEDS_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(SEEDS_BIRTH, SEEDS_BIRTH+size_next_state_vectors);
		break;
	case SIERPINSKI:
		NextStateIfLiving.assign(SIERPINSKI_SURVIVE, SIERPINSKI_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(SIERPINSKI_BIRTH, SIERPINSKI_BIRTH+size_next_state_vectors);
		break;
	case CORAL:
		NextStateIfLiving.assign(CORAL_SURVIVE, CORAL_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(CORAL_BIRTH, CORAL_BIRTH+size_next_state_vectors);
		break;
	case ASSIMILATION:
		NextStateIfLiving.assign(ASSIMILATION_SURVIVE, ASSIMILATION_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(ASSIMILATION_BIRTH, ASSIMILATION_BIRTH+size_next_state_vectors);
		break;
	case AMOEBA:
		NextStateIfLiving.assign(AMOEBA_SURVIVE, AMOEBA_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(AMOEBA_BIRTH, AMOEBA_BIRTH+size_next_state_vectors);
		break;
	case COAGULATION:
		NextStateIfLiving.assign(COAGULATION_SURVIVE, COAGULATION_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(COAGULATION_BIRTH, COAGULATION_BIRTH+size_next_state_vectors);
		break;
	case ICE:
		NextStateIfLiving.assign(ICE_SURVIVE, ICE_SURVIVE+size_next_state_vectors);
		NextStateIfDead.assign(ICE_BIRTH, ICE_BIRTH+size_next_state_vectors);
		break;
	}
}

//checks if index is valid, sets alive/dead state, sees if value changed, and tells neighbors to adjust their counts if this changed
bool cBoard::setCell(int row, int col, bool alive)
{
	if(isValidElement(row, col))
	{
		bool previous_state = field[row][col].living;
		field[row][col].living = alive;
		if(!previous_state && alive) //if changed from dead to alive
		{
			AdjustAdjacent(row, col, 1);//increment the count of nearby squares
			field[row][col].changed = true;
			if(viewNotify)
				viewNotify->NotifyCellChanged(row, col, alive);
		}
		else if(previous_state && !alive) //if changed from alive to dead
		{
			AdjustAdjacent(row, col, -1); //decrement the adjacency value of nearby squares
			field[row][col].changed = true;
			if(viewNotify)
				viewNotify->NotifyCellChanged(row, col, alive);
		}
		//else, didnt change, no need to update nearby cells
		return true;
	}
	return false;
}

void cBoard::setCellQuick(int row, int col, bool alive)
{
	bool previous_state = field[row][col].living;
	field[row][col].living = alive;
	//if changed from dead to alive or alive to dead
	if((!previous_state && alive) | (previous_state && !alive))
	{
		field[row][col].changed = true;
		++cells_handled_last_evolve;
		if(viewNotify)
			viewNotify->NotifyCellChanged(row, col, alive);
	}
}
cell cBoard::getCell(int row, int col) const
{
	if(!isValidElement(row, col))
	{
		std::runtime_error("Row or Column out of bounds, in cBoard::getCell. Dummy value returned.");
		cell temp = {false, 0};
		return temp;
	}
	else
		return field[row][col];
}
bool cBoard::invertCell(int row, int col) 
{
	if(isValidElement(row, col))
	{
		return setCell(row, col, !field[row][col].living); //useful to call set cause it updates nearby cells
	}
	else 
	{
		std::runtime_error("Row or Column out of bounds, in cBoard::invertCell. Operation Ignored.");
		return false;
	}
}
bool cBoard::isValidElement(int row, int col) const
{
	return (row < m_rows && col < m_columns && row >= 0 && col >= 0);
	//sanity check, if 0,0 and board has no elements, will return false
}

void cBoard::countBoard()
{
	TraverseAllIndices([&] (int row, int col) 
	{ 
		int oldcount(field[row][col].adjacent);
		int newcount(countAdjacent(row,col));
		if(oldcount != newcount)
		{
			field[row][col].adjacent = newcount;
			field[row][col].changed = true;
		}
	} );
}

void cBoard::Evolve(unsigned int num)
{
	while(num)
	{
		evolveOnce();
		--num;
	}
}

bool cBoard::evolveOnce()
{
	cells_handled_last_evolve = 0;

	TraverseAllIndices( [&] (int row, int col)		
	{
		if(field[row][col].changed)
		{			
			field[row][col].changed = false; 
			if(field[row][col].living)
			{
				setCellQuick(row, col, NextStateIfLiving[field[row][col].adjacent]);
			}
			else //if dead
			{
				setCellQuick(row, col, NextStateIfDead[field[row][col].adjacent]);
			}
		}
	} );

	if(controlNotify)
	{		
		controlNotify->NotifyEvolveFinished();
	}
	countBoard();

	return true;
}
void cBoard::OnEvolveTimer(wxTimerEvent& evt)
{
	Evolve(); 
	BeginEvolveTimer(); 
	//the timer is one shot and restarted after each evolve in case the timer is faster than
	//the board can process, to prevent an ever growing backlog of timer events. 
	//wxwidgets might somehow limit timer events from doing that but 
	//I don't know so I'm doing it this way to be safe
}
void cBoard::BeginEvolveTimer()
{
	if (evolve_speed > 0 )
		evolveTimer.Start(1000 / evolve_speed, true);
	else
		evolveTimer.Stop();
}

void cBoard::setBoardSize(int rows, int columns)
{
	if (rows > 0 && columns > 0)
	{
		field.resize(boost::extents[rows][columns]);

		//this is how it would be done with a vector of vector of cells if you dont have boost and are trying to recompile
		//field.resize(rows);
		//for(auto itr = field.begin(); itr != field.end(); ++itr)
		//{
		//	itr->resize(columns);
		//}

		m_rows = rows;
		m_columns = columns;
		clearBoard(); //have to clear cause if board was made larger, there is junk data in the new spots
	}
}

void cBoard::clearBoard()
{
	TraverseAllCells([] (cell& c) {c.living = false, c.adjacent = 0, c.changed = true;} );
	if(viewNotify)
	{
		TraverseAllIndices([&] (int row, int col) {			
			viewNotify->NotifyCellChanged(row, col, field[row][col].living);} );
	}
}

void cBoard::SetChangedFlags(bool val)
{
	TraverseAllCells( [&] (cell&c) {c.changed = val; } );
}

void cBoard::setWithPattern(int row, int col, std::array<bool, 9> pattern)
{
	if(m_rows >= 3 && m_columns >= 3) //can't draw on board smaller than 3x3
	{
		//true if valid bounded position for a 3x3 object centered at row, col and board is bounded, or true if board isn't bounded
		if ((m_bounded && row >= 1 && row < (m_rows - 1) && col >= 1 && col < (m_columns - 1)) || !m_bounded)
		{
			//glider pattern
			//0 1 0
			//0 0 1
			//1 1 1
			//not split up by rows, 0 1 0 0 1 1 1 1 x(0), adjacent only, x is the center, put center at the end
			int index(0);
			TraverseAdjacent(row, col, [&] (int row, int col) 
			{ 
				setCell(row, col, pattern[index]);
				++index;
			} );
			setCell(row, col, pattern[index]);
		}
	}
	//if bounded board and too close to edges, or board isnt big enough, can't make a glider there, do nothing
}

void cBoard::fillBoard()
{
	TraverseAllCells([] (cell&c) {c.living = true;} );

	countBoard();
}

void cBoard::AdjustAdjacent(int row, int col, int val)
{
	TraverseAdjacent(row, col, [&](int row, int col) {field[row][col].adjacent += val; field[row][col].changed = true;} );
}

int cBoard::countAdjacent(int row, int col) const
{
	int total(0);
	TraverseAdjacent(row, col, [&](int row, int col) {if(field[row][col].living) ++total;} );
	return total;
}

void cBoard::TraverseAdjacent(int row, int col, std::function<void (int row, int col)> func) const
{
	m_bounded ? TraverseAdjacentBounded(row, col, func ) 
		: TraverseAdjacentUnBounded(row, col, func );
}
void cBoard::TraverseAdjacentBounded(int row, int col, std::function<void (int row, int col)> func) const
{
	int left(col - 1), right(col + 1), above(row - 1), below(row + 1);

	//these have been ordered to go around the center from upper left to lower right, like so:
	//1 2 3
	//4 x 5
	//6 7 8
	if (above >= 0)
	{
		if (left >= 0)
			func(above, left);	//left and above
		func(above, col);		//directly above
		if (right < m_columns)
			func(above, right); //right and up
	}

	if (left >= 0)
		func(row, left);  //directly left
	if (right < m_columns)
		func(row, right); //directly right


	if (below < m_rows)
	{
		if (left >= 0)
			func(below, left);	//left and below
		func(below, col);		//directly below
		if (right < m_columns)
			func(below, right);	//right and below
	}
}
void cBoard::TraverseAdjacentUnBounded(int row, int col, std::function<void (int row, int col)> func) const
{
	int left(col - 1), right(col + 1), above(row - 1), below(row + 1);
	if (left < 0) //check the three to the left, if in array bounds
		left = m_columns - 1; //three to the left not valid, change to the farthest right side
	if (right >= m_columns) //check the three to the right, if in array bounds
		right = 0; //right side out of bounds, switch to left
	if (below >= m_rows) //check if below is out of bounds
		below = 0; //if below runs off the bottom of the array, use the top
	if (above < 0) //if above is out of bounds
		above = m_rows - 1; //use the bottom of the array

	//these have been ordered to go around the center from upper left to lower right, like so:
	//1 2 3
	//4 x 5
	//6 7 8
	func(above, left); //left and above
	func(above, col); 	//check directly above
	func(above, right); //right and up
	func(row, left);  //directly left
	func(row, right); //directly right
	func(below, left); //left and below
	func(below, col); //check directly below
	func(below, right); //right and down

}

void cBoard::TraverseAllCells(std::function<void (cell& c)> func) 
{
	for(auto itr_rows = field.begin(); itr_rows != field.end(); ++itr_rows)
	{
		for(auto itr_cols = itr_rows->begin(); itr_cols != itr_rows->end(); ++itr_cols)
		{
			func(*itr_cols);
		}
	}
}
void cBoard::TraverseAllIndices(std::function<void (int row, int col)> func) const
{
	for(std::size_t row(0); row < field.size(); ++row)
	{
		for(std::size_t col(0); col < field[row].size(); ++col)
		{	
			func(row, col);
		}
	}
}
