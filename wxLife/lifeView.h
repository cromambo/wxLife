#pragma once
#include "wx/wx.h"
#include "cBoard.h"

#ifdef __WXMSW__ //this shows any memory leaks on exit
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif 

class lifeView : public wxPanel
{
public:
	lifeView(wxFrame* parent, std::shared_ptr<const cBoard> boardPtr);
	~lifeView();

	//this makes an empty bmp of the right size
	//and since its being drawn directly to the window, reserves some empty space in the sizer for it to draw to
	void ResizeBitmap(int rows, int cols);
	void NotifyCellChanged(int row, int col, bool alive);
	void RedrawFull();
	void RedrawChanges();
	void SetBoardPtr(std::shared_ptr<const cBoard> board);
	void ReleaseBoardPtr();
	bool FindMousePositionOnBoard(wxMouseEvent& evt, int& row, int& col) const;
	void Highlight(int row, int col);
	void HighlightOff();
	int getCellSize() const;
	void setCellSize(int size);
private:
	//shared pointer so if passed board is released/out of scope from caller, this holds onto it to display 
	//(and so the program doesnt crash)
	//const so this never changes the board, just accesses it with getters
	//the passed in board doesnt have to be const, only this object considers it so, since this pointer accesses
	//it as a "pointer to a constant object", but other pointers can be "pointer to a object"
	std::shared_ptr<const cBoard> board;

	wxBitmap boardBMP;  //buffer for drawing board onto
	int cellsize;
	bool highlight;
	int highlight_row, highlight_col;
	int prev_highlight_row, prev_highlight_col;
	wxBoxSizer* boardSizer;

	lifeView(const lifeView& view);

	void DrawBoard(wxDC &dc, bool full_redraw = false);
	void CreateBitmapFromBoard(bool full_redraw = false);
	void UpdateHighlight();
	void DrawHighlight(wxDC &dc);
	void ClearHighlight(wxDC &dc);

	//event ids
	enum { ID_Draw_Frame};

	//event handlers
	void OnMouseCaptureLost(wxMouseCaptureLostEvent& evt);
	void OnPaint(wxPaintEvent& evt);
	void OnLeaveWindow(wxMouseEvent& evt);
	void StopFlicker(wxEraseEvent& evt);
};
