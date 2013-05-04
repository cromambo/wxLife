#include "cBoardUnitTest.h"


cBoardUnitTest::cBoardUnitTest(void)
{
}


cBoardUnitTest::~cBoardUnitTest(void)
{
}
void cBoardUnitTest::testCountAdjacentBounded()
{
	//set size 3x3
	testboard.setBoardSize(3, 3);
	testboard.setBounded(true);

	//clear and see if that worked
	testboard.clearBoard();
	assert(testboard.getCell(0, 0).living == false);
	assert(testboard.getCell(0, 1).living == false);
	assert(testboard.getCell(0, 2).living == false);
	assert(testboard.getCell(1, 0).living == false); 
	assert(testboard.getCell(1, 1).living == false);
	assert(testboard.getCell(1, 2).living == false);
	assert(testboard.getCell(2, 0).living == false);
	assert(testboard.getCell(2, 1).living == false);
	assert(testboard.getCell(2, 2).living == false);


	//count with everything dead, all results should be 0
	//written out instead of a for loop because assert failure message doesnt show the x,y value
	assert(testboard.countAdjacent(0, 0) == 0);
	assert(testboard.countAdjacent(0, 1) == 0);
	assert(testboard.countAdjacent(0, 2) == 0);
	assert(testboard.countAdjacent(1, 0) == 0); 
	assert(testboard.countAdjacent(1, 1) == 0);
	assert(testboard.countAdjacent(1, 2) == 0);
	assert(testboard.countAdjacent(2, 0) == 0);
	assert(testboard.countAdjacent(2, 1) == 0);
	assert(testboard.countAdjacent(2, 2) == 0);

	//fill board with all alive
	// a a a
	// a a a
	// a a a
	testboard.fillBoard();
	assert(testboard.getCell(0, 0).living == true);
	assert(testboard.getCell(0, 1).living == true);
	assert(testboard.getCell(0, 2).living == true);
	assert(testboard.getCell(1, 0).living == true); 
	assert(testboard.getCell(1, 1).living == true);
	assert(testboard.getCell(1, 2).living == true);
	assert(testboard.getCell(2, 0).living == true);
	assert(testboard.getCell(2, 1).living == true);
	assert(testboard.getCell(2, 2).living == true);

	//expected results of adjacent living cells for each spot of the filled board:
	//3 5 3
	//5 8 5
	//3 5 3
	assert(testboard.countAdjacent(0, 0) == 3);
	assert(testboard.countAdjacent(0, 1) == 5);
	assert(testboard.countAdjacent(0, 2) == 3);
	assert(testboard.countAdjacent(1, 0) == 5);
	assert(testboard.countAdjacent(1, 1) == 8);
	assert(testboard.countAdjacent(1, 2) == 5);
	assert(testboard.countAdjacent(2, 0) == 3);
	assert(testboard.countAdjacent(2, 1) == 5);
	assert(testboard.countAdjacent(2, 2) == 3);

	//next test pattern, for some 1s and 2s
	//d a d  1 1 2
	//d d a  2 3 2
	//d a d  1 1 2
	testboard.clearBoard();
	testboard.setCell(0, 0, false);
	testboard.setCell(0, 1, true);
	testboard.setCell(0, 2, false);
	testboard.setCell(1, 0, false);
	testboard.setCell(1, 1, false);
	testboard.setCell(1, 2, true);
	testboard.setCell(2, 0, false);
	testboard.setCell(2, 1, true);
	testboard.setCell(2, 2, false);

	//test all the set cells worked
	assert(testboard.getCell(0, 0).living == false);
	assert(testboard.getCell(0, 1).living == true);
	assert(testboard.getCell(0, 2).living == false);
	assert(testboard.getCell(1, 0).living == false); 
	assert(testboard.getCell(1, 1).living == false);
	assert(testboard.getCell(1, 2).living == true);
	assert(testboard.getCell(2, 0).living == false);
	assert(testboard.getCell(2, 1).living == true);
	assert(testboard.getCell(2, 2).living == false);

	assert(testboard.countAdjacent(0, 0) == 1);
	assert(testboard.countAdjacent(0, 1) == 1);
	assert(testboard.countAdjacent(0, 2) == 2);
	assert(testboard.countAdjacent(1, 0) == 2);
	assert(testboard.countAdjacent(1, 1) == 3);
	assert(testboard.countAdjacent(1, 2) == 2);
	assert(testboard.countAdjacent(2, 0) == 1);
	assert(testboard.countAdjacent(2, 1) == 1);
	assert(testboard.countAdjacent(2, 2) == 2);
}

void cBoardUnitTest::testCountAdjacentUnBounded()
{
	//set size 3x3
	testboard.setBoardSize(3, 3);
	testboard.setBounded(false);

	//clear and see if that worked
	testboard.clearBoard();
	assert(testboard.getCell(0, 0).living == false);
	assert(testboard.getCell(0, 1).living == false);
	assert(testboard.getCell(0, 2).living == false);
	assert(testboard.getCell(1, 0).living == false); 
	assert(testboard.getCell(1, 1).living == false);
	assert(testboard.getCell(1, 2).living == false);
	assert(testboard.getCell(2, 0).living == false);
	assert(testboard.getCell(2, 1).living == false);
	assert(testboard.getCell(2, 2).living == false);


	//count with everything dead, all results should be 0
	//written out instead of a for loop because assert failure message doesnt show the x,y value
	assert(testboard.countAdjacent(0, 0) == 0);
	assert(testboard.countAdjacent(0, 1) == 0);
	assert(testboard.countAdjacent(0, 2) == 0);
	assert(testboard.countAdjacent(1, 0) == 0); 
	assert(testboard.countAdjacent(1, 1) == 0);
	assert(testboard.countAdjacent(1, 2) == 0);
	assert(testboard.countAdjacent(2, 0) == 0);
	assert(testboard.countAdjacent(2, 1) == 0);
	assert(testboard.countAdjacent(2, 2) == 0);

	//fill board with all alive
	// a a a
	// a a a
	// a a a
	testboard.fillBoard();
	assert(testboard.getCell(0, 0).living == true);
	assert(testboard.getCell(0, 1).living == true);
	assert(testboard.getCell(0, 2).living == true);
	assert(testboard.getCell(1, 0).living == true); 
	assert(testboard.getCell(1, 1).living == true);
	assert(testboard.getCell(1, 2).living == true);
	assert(testboard.getCell(2, 0).living == true);
	assert(testboard.getCell(2, 1).living == true);
	assert(testboard.getCell(2, 2).living == true);

	//expected results of adjacent living cells for each spot of the filled board:
	assert(testboard.countAdjacent(0, 0) == 8);
	assert(testboard.countAdjacent(0, 1) == 8);
	assert(testboard.countAdjacent(0, 2) == 8);
	assert(testboard.countAdjacent(1, 0) == 8);
	assert(testboard.countAdjacent(1, 1) == 8);
	assert(testboard.countAdjacent(1, 2) == 8);
	assert(testboard.countAdjacent(2, 0) == 8);
	assert(testboard.countAdjacent(2, 1) == 8);
	assert(testboard.countAdjacent(2, 2) == 8);

	//next test pattern
	//d a d  3 2 3
	//d d a  3 3 2
	//d a d  3 2 3
	testboard.clearBoard();
	testboard.setCell(0, 0, false);
	testboard.setCell(0, 1, true);
	testboard.setCell(0, 2, false);
	testboard.setCell(1, 0, false);
	testboard.setCell(1, 1, false);
	testboard.setCell(1, 2, true);
	testboard.setCell(2, 0, false);
	testboard.setCell(2, 1, true);
	testboard.setCell(2, 2, false);

	//test all the set cells worked
	assert(testboard.getCell(0, 0).living == false);
	assert(testboard.getCell(0, 1).living == true);
	assert(testboard.getCell(0, 2).living == false);
	assert(testboard.getCell(1, 0).living == false); 
	assert(testboard.getCell(1, 1).living == false);
	assert(testboard.getCell(1, 2).living == true);
	assert(testboard.getCell(2, 0).living == false);
	assert(testboard.getCell(2, 1).living == true);
	assert(testboard.getCell(2, 2).living == false);

	assert(testboard.countAdjacent(0, 0) == 3);
	assert(testboard.countAdjacent(0, 1) == 2);
	assert(testboard.countAdjacent(0, 2) == 3);
	assert(testboard.countAdjacent(1, 0) == 3);
	assert(testboard.countAdjacent(1, 1) == 3);
	assert(testboard.countAdjacent(1, 2) == 2);
	assert(testboard.countAdjacent(2, 0) == 3);
	assert(testboard.countAdjacent(2, 1) == 2);
	assert(testboard.countAdjacent(2, 2) == 3);
}
