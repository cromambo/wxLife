#include "lifeControl.h"
#ifdef __WXMSW__ //this shows any memory leaks on exit
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif 

lifeControl::lifeControl(const wxString& title, const wxPoint& pos, const wxSize& size)
	:wxFrame (NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE 
	& ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)
	), 
	paused(false), prev_speed(10), 	manual_pause(false),
	board(std::make_shared<cBoard>(50,50)),//board(new cBoard()) would also work, but make_shared reduces construction overhead
	menuBar(nullptr),
	menuFile(nullptr),
	toolbar(nullptr),
	checkboxBounded(nullptr),
	speedSlider(nullptr),
	zoomSlider(nullptr),
	viewPanel(nullptr),
	sizer(nullptr),
	controlSizer(nullptr),
	textEntrySizer(nullptr),
	rowTextCtrl(nullptr),
	colTextCtrl(nullptr),
	pauseButton(nullptr),
	resizeButton(nullptr),
	clrButton(nullptr),
	advButton(nullptr),
	statusBar(nullptr),
	ruleChoice(nullptr)
{
	ConfigureMenus();
	ConfigureIcon();
	ConfigureStatusBar();
	ConfigureSliders();
	ConfigureCheckboxes();
	ConfigureView(); //call after checkboxes and sliders set up to have correct initial values
	ConfigureChoiceDropdowns();

	if(board)
	{
		board->SetSpeed(speedSlider->GetValue());
		board->setBounded(!checkboxBounded->GetValue()); //wrap edges true means bounded false
		board->setControlNotificationTarget(this);
	}
	ConfigureResize();
	ConfigureButtons();

	ConfigureSizers(); //make sure to call this only after all the other configs have been called

	viewPanel->Bind(wxEVT_KEY_DOWN, &lifeControl::OnKeyDown, this); 	//keyboard controls
	viewPanel->Bind(wxEVT_MOUSEWHEEL, &lifeControl::OnMouseWheel, this);  

	viewPanel->SetFocus();

	UpdateView();
}

lifeControl::~lifeControl()
{
	/*from http://wiki.wxwidgets.org/Avoiding_Memory_Leaks
	"When a wxWindow is destroyed, it automatically deletes all its 
	children. These children are all the objects that received the 
	window as the parent-argument in their constructors.
	As a consequence, if you're creating a derived class that contains child
	windows, you should use a pointer to the child windows instead of the objects 
	themself as members of the main window."*/

	//hence, nothing needs to be deleted here
}

void lifeControl::ConfigureMenus()
{
	//menus
	menuFile = new wxMenu;

	menuFile->AppendSeparator();
	menuFile->Append( ID_Quit, _("&Quit"));
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append( ID_Explain, _("&Game Explanation"));
	menuHelp->Append( ID_Controls, _("&Controls"));
	menuHelp->Append( ID_About, _("&About..."));
	menuBar = new wxMenuBar;
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuHelp, _("&Help"));
	SetMenuBar(menuBar);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &lifeControl::OnQuit, this, ID_Quit); 
	Bind(wxEVT_COMMAND_MENU_SELECTED, &lifeControl::OnAbout, this, ID_About);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &lifeControl::OnControls, this, ID_Controls); 
	Bind(wxEVT_COMMAND_MENU_SELECTED, &lifeControl::OnExplain, this, ID_Explain);
}
void lifeControl::ConfigureIcon()
{
	wxImage::AddHandler(new wxPNGHandler); //image handler so pngs can be opened
	SetIcon(wxIcon(_("resources/iconlife.png"), wxBITMAP_TYPE_PNG)); 	//corner icon
}
void lifeControl::ConfigureChoiceDropdowns()
{
	const int num_choices(11);
	wxString choices[num_choices] = {
		_("Conways(B3/S23)"), 
		_("MazeFill(B3/S12345)"), 
		_("Move(B368/S245)"), 
		_("Highlife(B36/S23)"),
		_("Seeds(B3/S)"), 
		_("Sierpinski(B1/S12)"),
		_("Coral(B3/S45678)"), 
		_("Assimilation(B345/S4567)"), 
		_("Amoeba(B357/S1358)"), 
		_("Coagulation(B367/S125678)"),
		_("Ice(B3678/S235678)")};
	ruleChoice = new wxChoice(this, ID_RuleChoice, wxDefaultPosition, wxDefaultSize, num_choices, choices);
	ruleChoice->Select(0);
	board->setEvolveRule(cBoard::CONWAY);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED , &lifeControl::OnRuleChoice, this, ID_RuleChoice); 
}
void lifeControl::ConfigureStatusBar()
{
	statusBar = new wxStatusBar (this);
	statusBar->SetFieldsCount(3);
	statusBar->SetStatusText(_("Running"), ISRUNNING);
	this->SetStatusBar(statusBar);
}
void lifeControl::ConfigureCheckboxes()
{
	checkboxBounded = new wxCheckBox(this, ID_Bounded, _("Wrap Edges"));
	checkboxBounded->SetValue(true);

	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &lifeControl::OnBoundedCheck, this, ID_Bounded); //wrap edges checkbox
}
void lifeControl::ConfigureSliders()
{
	int startzoom(20), startspeed(10);
	zoomSlider = new wxSlider(this, ID_ZoomSlider, startzoom, 5, 40, wxDefaultPosition, wxDefaultSize );
	speedSlider = new wxSlider(this, ID_SpeedSlider, startspeed, 0, 100, wxDefaultPosition, wxDefaultSize );
	Bind(wxEVT_COMMAND_SLIDER_UPDATED, &lifeControl::OnSpeedSliderScroll, this, ID_SpeedSlider); //slider
	Bind(wxEVT_COMMAND_SLIDER_UPDATED, &lifeControl::OnZoomSliderScroll, this, ID_ZoomSlider); //slider
}
void lifeControl::ConfigureView()
{
	viewPanel = new lifeView(this, board); //the panel with the game of life board, this is where the magic happens
	viewPanel->setCellSize(zoomSlider->GetValue());
	//redirect mouse events from the board display to this, only this can change the board's data.
	viewPanel->Bind(wxEVT_LEAVE_WINDOW, &lifeControl::OnLeaveWindow, this);
	viewPanel->Bind(wxEVT_LEFT_DOWN, &lifeControl::OnLeftClickDown, this);	
	viewPanel->Bind(wxEVT_RIGHT_DOWN, &lifeControl::OnRightClickDown, this);
	viewPanel->Bind(wxEVT_MOTION, &lifeControl::OnMouseMotion, this);
	viewPanel->Bind(wxEVT_LEFT_UP, &lifeControl::OnLeftClickUp, this);
	viewPanel->Bind(wxEVT_RIGHT_UP, &lifeControl::OnRightClickUp, this);
	if(board)
		board->setViewNotificationTarget(viewPanel);
}
void lifeControl::ConfigureResize()
{
	//todo?, press enter to resize, wxEVT_COMMAND_TEXT_ENTER sent if wxTE_PROCESS_ENTER flag for text control
	rowTextCtrl = new wxTextCtrl(this, -1, wxString()<<board->getRows(), wxDefaultPosition, wxSize(30, 20), 0L, wxIntegerValidator<int>() );
	colTextCtrl = new wxTextCtrl(this, -1, wxString()<<board->getColumns(), wxDefaultPosition, wxSize(30, 20), 0L, wxIntegerValidator<int>());
	resizeButton = new wxButton(this, ID_Resize, _("Resize"));
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &lifeControl::OnResize, this, ID_Resize); //resize button

	textEntrySizer = new wxGridSizer(2,2,0,0);
	textEntrySizer->Add(new wxStaticText(this, -1, _("Row")));
	textEntrySizer->Add(rowTextCtrl, 1);
	textEntrySizer->Add(new wxStaticText(this, -1, _("Col")), 1);
	textEntrySizer->Add(colTextCtrl, 1);
}
void lifeControl::ConfigureButtons()
{
	pauseButton = new wxButton(this, ID_Pause, _("Pause"));
	clrButton = new wxButton(this, ID_Clear, _("Clear"));
	advButton = new wxButton(this, ID_Advance, _("Advance"));
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &lifeControl::OnClear, this, ID_Clear); //clear button
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &lifeControl::OnAdvance, this, ID_Advance); //advance button	
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &lifeControl::OnPause, this, ID_Pause); //pause button	
}
void lifeControl::ConfigureSizers()
{
	//sizer for all the buttons and sliders
	controlSizer = new wxBoxSizer(wxVERTICAL);
	controlSizer->Add(clrButton);

	controlSizer->Add(ruleChoice);
	controlSizer->Add(new wxStaticText(this, -1, _("Zoom:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT, _("ZoomText")));
	controlSizer->Add(zoomSlider);
	controlSizer->Add(textEntrySizer);	
	controlSizer->Add(resizeButton);
	controlSizer->Add(checkboxBounded);
	controlSizer->Add(new wxStaticText(this, -1, _("Speed:")));
	controlSizer->Add(speedSlider);
	controlSizer->Add(pauseButton);
	controlSizer->Add(advButton);




	//top level sizer
	sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(controlSizer, 0, wxALIGN_TOP | wxALIGN_LEFT);
	sizer->Add(viewPanel);
	sizer->SetSizeHints(this);
	this->SetSizer(sizer);
}
void lifeControl::OnQuit(wxCommandEvent& evt)
{
	Close(true);
}
void lifeControl::OnAbout(wxCommandEvent& evt)
{
	wxMessageBox( _("Written by Sam Mancill, April 2013. Used to learn wxWidgets."), _("About"), wxOK | wxICON_INFORMATION, this);
}
void lifeControl::OnControls(wxCommandEvent& evt)
{
	wxMessageBox( _("Left Click on board to toggle a cell living or dead. Left Click, hold and drag to draw a line of living cells. Right click or Hold down right mouse to erase cells. Shift Left click to make a glider. Space to Pause. Mouse scroll wheel zooms in and out"), 
		_("Controls."), wxOK | wxICON_INFORMATION, this);
}
void lifeControl::OnExplain(wxCommandEvent& evt)
{
	wxMessageBox( _("From Wikipedia: The universe of the Game of Life is an infinite two-dimensional "
		"orthogonal grid of square cells, each of which is in one of two possible states, alive or dead. "
		"Every cell interacts with its eight neighbours, which are the cells that are horizontally, "
		"vertically, or diagonally adjacent. At each step in time, the following transitions occur: "
		"Any live cell with fewer than two live neighbours dies, as if caused by under-population. "
		"Any live cell with two or three live neighbours lives on to the next generation. "
		"Any live cell with more than three live neighbours dies, as if by overcrowding. "
		"Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction. "
		"The game is a zero-player game, meaning that its evolution is determined by its initial state,"
		"requiring no further input. One interacts with the Game of Life by creating an initial "
		"configuration and observing how it evolves."), 
		_("Game Explanation"), wxOK | wxICON_INFORMATION, this);
}
void lifeControl::OnClear(wxCommandEvent& evt)
{
	board->clearBoard();
	viewPanel->SetFocus();
}

void lifeControl::OnRuleChoice(wxCommandEvent& evt)
{
	switch(evt.GetSelection())
	{
	case 0:
		board->setEvolveRule(cBoard::CONWAY);
		break;
	case 1:
		board->setEvolveRule(cBoard::MAZE);
		break;
	case 2:
		board->setEvolveRule(cBoard::MOVE);
		break;
	case 3:
		board->setEvolveRule(cBoard::HIGHLIFE);
		break;
	case 4:
		board->setEvolveRule(cBoard::SEEDS);
		break;
	case 5:
		board->setEvolveRule(cBoard::SIERPINSKI);
		break;
	case 6:
		board->setEvolveRule(cBoard::CORAL);
		break;
	case 7:
		board->setEvolveRule(cBoard::ASSIMILATION);
		break;
	case 8:
		board->setEvolveRule(cBoard::AMOEBA);
		break;
	case 9:
		board->setEvolveRule(cBoard::COAGULATION);
		break;
	case 10:
		board->setEvolveRule(cBoard::ICE);
		break;
	}
	board->SetChangedFlags(true); //make all elements be used in next calc because the rule is new
}

void lifeControl::OnZoomSliderScroll(wxCommandEvent& evt)
{
	ChangeZoom(evt.GetInt());
	viewPanel->SetFocus();
}

void lifeControl::OnBoundedCheck(wxCommandEvent& evt)
{
	board->setBounded(!evt.IsChecked());
}
void lifeControl::ChangeZoom(int zoom)
{
	zoom = (zoom > 0) ? zoom : 0;
	if(viewPanel->getCellSize() != zoom)
	{
		viewPanel->setCellSize(zoom);
		//something to limit size to fit on screen?
		//cellsize = (cellsize < 100) ? cellsize : 100;

		UpdateView();
	}
}

void lifeControl::OnResize(wxCommandEvent& evt)
{
	ChangeBoardSize(wxAtoi(rowTextCtrl->GetValue()), wxAtoi(colTextCtrl->GetValue()));
	viewPanel->SetFocus();
}
void lifeControl::ChangeBoardSize(int rows, int cols)
{
	board->setBoardSize(rows, cols);
	UpdateView();
}
//to be called after anything that changes the boards size or zoom
void lifeControl::UpdateView()
{
	viewPanel->ResizeBitmap(board->getRows(), board->getColumns());
	Layout();
	Fit();
}
void lifeControl::OnKeyDown(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode())
	{
	case WXK_SPACE:
		TogglePaused();

		break;
	}
	//evt.DoAllowNextEvent(); //trying to let it pass through to text controls
	//evt.Skip();
}
void lifeControl::OnAdvance(wxCommandEvent& evt)
{
	board->Evolve(1);
	UpdateChangeCounter();
	viewPanel->SetFocus();
}
void lifeControl::NotifyEvolveFinished()
{
	UpdateChangeCounter();
}
void lifeControl::UpdateChangeCounter()
{
	statusBar->SetStatusText(wxString()<<"Cells changed last evolution: " << board->getCellsHandledLastEvolve(), EVOLVECOUNT);
}

void lifeControl::OnPause(wxCommandEvent& evt)
{
	TogglePaused();
	viewPanel->SetFocus();
}

void lifeControl::TogglePaused()
{
	manual_pause = !manual_pause;
	(paused) ? Unpause() :Pause();
}

void lifeControl::Pause()
{
	if(!paused)
	{
		prev_speed = speedSlider->GetValue();
		speedSlider->SetValue(0);
		board->SetSpeed(0);
		pauseButton->SetLabel("Resume");
		statusBar->SetStatusText(_("Paused"), ISRUNNING);
		paused = true;
	}
}

void lifeControl::Unpause()
{
	if(paused)
	{
		speedSlider->SetValue(prev_speed);
		board->SetSpeed(prev_speed);
		pauseButton->SetLabel("Pause");
		statusBar->SetStatusText(_("Running"), ISRUNNING);
		paused = false;
	}
}

void lifeControl::OnSpeedSliderScroll(wxCommandEvent& evt)
{
	int spd = evt.GetInt();

	spd = (spd > 0) ? spd : 0; //make sure its positive
	prev_speed = spd; //for resuming same speed when pausing/unpausing (which sets speed to 0)
	//todo, limit max speed?

	board->SetSpeed(spd);
	if(spd > 0)
		Unpause();

	viewPanel->SetFocus();
}

void lifeControl::SetWithPattern(int row, int col)
{
	//todo: add pattern selector element, pick pattern based on it
	std::array<bool, 9> v = PATTERN_GLIDER_DOWN_RIGHT;
	board->setWithPattern(row, col, v );
}

void lifeControl::OnMouseMotion(wxMouseEvent& evt)
{
	int row(0), col(0);
	if(viewPanel->FindMousePositionOnBoard(evt, row, col))
	{
		statusBar->SetStatusText(wxString()<<"Row: "<< row+1 << " Column: " << col+1, MOUSEPOSITION); //adding one so user doesnt see things zero-indexed
		if(evt.Dragging())
		{
			if(evt.LeftIsDown())
			{
				if(evt.ShiftDown())
				{
					SetWithPattern(row, col);
				}
				else
				{
					board->setCell(row, col, true);
				}
				viewPanel->HighlightOff();
			}
			else if (evt.RightIsDown())
			{
				viewPanel->Highlight(row, col);
				board->setCell(row, col, false);				
			}
			//dont force a redraw here because the frame timer does it "fps" times a second
		}
		else if(evt.Moving()) //might not need this "if", not sure if there are other states, dragging or moving i think only one should be true at a time
		{
			viewPanel->Highlight(row, col);
		}
	}
	else
	{
		viewPanel->HighlightOff();
	}
	evt.Skip(); //let default handling happen too
}

void lifeControl::OnMouseWheel(wxMouseEvent& evt)
{
	int zoom = zoomSlider->GetValue();

	if (evt.GetWheelRotation() > 0 && zoom != zoomSlider->GetMax()) 
		++zoom;
	if (evt.GetWheelRotation() < 0 && zoom != zoomSlider->GetMin())
		--zoom;

	zoomSlider->SetValue(zoom);
	ChangeZoom(zoom);
	viewPanel->SetFocus();
}
void lifeControl::OnLeftClickDown(wxMouseEvent& evt)
{
	//	note on mouse capture:
	//i put this in thinking it would limit the mouse position to the board (aka make it "stick" when trying to go over the edges)
	//thats not what it does but it does restricting dragging events to this window instead of spreading them all over
	//and it felt common to me to run off the edges of the board drawing some lines, so i left it in to not spam any other windows
	//with mousemotion dragging events. While the mouse is captured, keypresses will not be passed up to the frame window
	//so between down and up clicks, like drawing on the board, keyboard shortcuts written at a higher level will not work

	int row(0), col(0);
	if(viewPanel->FindMousePositionOnBoard(evt, row, col))
	{
		Pause();
		viewPanel->CaptureMouse();
		if(evt.ShiftDown())
		{
			SetWithPattern(row, col);
		}
		else
		{
			board->invertCell(row, col);	
		}
		//dont force a redraw here because the frame timer does it "fps" times a second
	}
	evt.Skip(); //let default handling happen too
}
void lifeControl::OnRightClickDown(wxMouseEvent& evt)
{
	//assumption is that board starts at 0,0 corner of panel
	int row(0), col(0);
	if(viewPanel->FindMousePositionOnBoard(evt, row, col))
	{
		viewPanel->CaptureMouse();

		board->setCell(row, col, false);		
		viewPanel->HighlightOff();
		//dont force a redraw here because the frame timer does it "fps" times a second
	}
	evt.Skip(); //let default handling happen too
}

void lifeControl::OnLeftClickUp(wxMouseEvent& evt)
{
	int row(0), col(0);
	if(viewPanel->FindMousePositionOnBoard(evt, row, col))
	{
		viewPanel->Highlight(row, col);

		if(!manual_pause)
			Unpause();
	}
	if(viewPanel->HasCapture())
		viewPanel->ReleaseMouse();

	evt.Skip(); //let default handling happen too
}
void lifeControl::OnRightClickUp(wxMouseEvent& evt)
{
	if(viewPanel->HasCapture())
		viewPanel->ReleaseMouse();

	evt.Skip(); //let default handling happen too
}
void lifeControl::OnLeaveWindow(wxMouseEvent& evt)
{
	if(!manual_pause)
		Unpause(); 
	//when the mouse is dragged with left held, evolving is usually paused until the mouse is released
	//this event handler fixes the condition where the mouse is dragged off the screen so the LeftClickUp isnt seen,
	//and the paused state is not returned to what it was before the mouse motion
	statusBar->SetStatusText(_(" "), MOUSEPOSITION);
	evt.Skip(); //let default handling happen too
}
