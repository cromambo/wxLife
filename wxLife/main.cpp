#include "main.h"
#include "cBoardUnitTest.h"


bool lifeApp::OnInit()
{
	cBoardUnitTest tester;
	tester.testCountAdjacentBounded();
	tester.testCountAdjacentUnBounded();

	lifeControl *frame = new lifeControl( _("The Game of Life"), wxDefaultPosition, wxSize(470, 600) );
	frame->Show(true);
	SetTopWindow(frame);	
	return true;
};

IMPLEMENT_APP(lifeApp)
