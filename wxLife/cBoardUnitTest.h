#pragma once
#include "cBoard.h"

class cBoardUnitTest
{
protected:
	cBoard testboard;
public:
	void testCountAdjacentBounded();
	void testCountAdjacentUnBounded();
	cBoardUnitTest();
	~cBoardUnitTest();
};

