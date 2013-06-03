#include "SDL.h"
#include "cBoard.h"
#include <string>
#include "cScreen.h"
#include "cGameController.h"
#include "cBoardUnitTest.h"

int main( int argc, char* args[] )
{
	cBoardUnitTest tester;
	tester.testCountAdjacentBounded();


	cGameController gameinstance;
	gameinstance.run();


	return 0;    //have to return something when using SDL, can't use void main
}
