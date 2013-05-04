#pragma once
#include "wx/wx.h"
#include "cBoard.h"
#include "lifeView.h"
#include <string>
//#include <wx/valnum.h> //i added this to the wx/wx.h header in my wx include directoy, it wasnt in there with the distributed 2.9.4 binaries. support for IntegerValidator

class lifeControl :public wxFrame
{
public:
	lifeControl(const wxString& title, const wxPoint& pos, const wxSize& size);
	~lifeControl();

	void ChangeBoardSize(int rows, int cols);
	void TogglePaused();
	void NotifyEvolveFinished();

private:
	//various controls, pointers kept so they can be accessed and changed
	wxMenuBar *menuBar;
	wxMenu *menuFile;
	wxToolBar * toolbar;
	wxCheckBox* checkboxBounded;
	wxSlider* speedSlider;
	wxSlider* zoomSlider;
	lifeView* viewPanel;
	wxBoxSizer* sizer;
	wxBoxSizer* controlSizer;
	wxGridSizer* textEntrySizer;
	wxTextCtrl* rowTextCtrl;
	wxTextCtrl* colTextCtrl;
	wxButton* pauseButton;
	wxButton* resizeButton;
	wxButton* clrButton;
	wxButton* advButton;
	wxStatusBar* statusBar;
	wxChoice* ruleChoice;

	//some flags to track pausing, resuming from pause, and pausing during mouse drawing on board
	bool paused;
	bool manual_pause;
	int prev_speed;

	std::shared_ptr<cBoard> board;

	enum StatusBarFields{ISRUNNING = 0, EVOLVECOUNT = 1, MOUSEPOSITION = 2};

	lifeControl(const lifeControl& control);

	void UpdateView();
	void Pause();
	void Unpause();
	void UpdateChangeCounter();
	void SetWithPattern(int row, int col);
	void ChangeZoom(int zoom);

	void ConfigureMenus();
	void ConfigureIcon();
	void ConfigureSizers();
	void ConfigureSliders();
	void ConfigureResize();
	void ConfigureCheckboxes();
	void ConfigureButtons();
	void ConfigureView();
	void ConfigureStatusBar();
	void ConfigureChoiceDropdowns();
	//event IDs
	enum { ID_Quit, ID_About, ID_Clear, ID_Bounded, ID_Resize,
		ID_SpeedSlider, ID_ZoomSlider, ID_Advance, ID_Pause, ID_Explain, ID_Controls, ID_RuleChoice};

	//event handlers
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnClear(wxCommandEvent& evt);
	void OnSpeedSliderScroll(wxCommandEvent& evt);
	void OnZoomSliderScroll(wxCommandEvent& evt);
	void OnBoundedCheck(wxCommandEvent& evt);
	void OnKeyDown(wxKeyEvent& evt);
	void OnResize(wxCommandEvent& evt);
	void OnAdvance(wxCommandEvent& evt);
	void OnPause(wxCommandEvent& evt);
	void OnLeftClickDown(wxMouseEvent& evt);	
	void OnLeftClickUp(wxMouseEvent& evt);
	void OnRightClickDown(wxMouseEvent& evt);
	void OnRightClickUp(wxMouseEvent& evt);	
	void OnMouseMotion(wxMouseEvent& evt);
	void OnLeaveWindow(wxMouseEvent& evt);
	void OnControls(wxCommandEvent& evt);
	void OnExplain(wxCommandEvent& evt);	
	void OnMouseWheel(wxMouseEvent& evt);
	void OnRuleChoice(wxCommandEvent& evt);
};
