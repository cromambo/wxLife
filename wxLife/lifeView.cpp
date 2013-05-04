#include "lifeView.h"

lifeView::lifeView(wxFrame * parent, std::shared_ptr<const cBoard> boardPtr)
	: board(boardPtr),
	wxPanel(parent, wxID_ANY),
	highlight(false), highlight_row(0),highlight_col(0), cellsize(1),
	prev_highlight_row(0), prev_highlight_col(0), boardSizer(nullptr)
{
	boardSizer = new wxBoxSizer(wxHORIZONTAL);
	boardSizer->AddStretchSpacer();
	this->SetSizer(boardSizer);

	Bind(wxEVT_MOUSE_CAPTURE_LOST, &lifeView::OnMouseCaptureLost, this);
	Bind(wxEVT_PAINT, &lifeView::OnPaint, this);
	Bind(wxEVT_LEAVE_WINDOW, &lifeView::OnLeaveWindow, this);
	Bind(wxEVT_ERASE_BACKGROUND, &lifeView::StopFlicker, this);

	ResizeBitmap(board->getRows(), board->getColumns());
}

lifeView::~lifeView()
{
}
void lifeView::SetBoardPtr(std::shared_ptr<const cBoard> board)
{
	//A shared_ptr stops owning a resource when it is reassigned or reset.
	//so if a different board is passed in this stops referencing its previous board, if any, and uses the new one.
	lifeView::board = board;
}
void lifeView::ReleaseBoardPtr()
{
	lifeView::board = nullptr;
}
int lifeView::getCellSize() const
{
	return cellsize;
}
void lifeView::setCellSize(int size)
{
	cellsize = size;
}
void lifeView::StopFlicker(wxEraseEvent& evt)
{
	; //just here to intercept the wxEVT_ERASE_BACKGROUND event to stop flicker
}

void lifeView::OnLeaveWindow(wxMouseEvent& evt)
{
	highlight = false;
	UpdateHighlight();
	evt.Skip();
}
void lifeView::Highlight(int row, int col)
{
	highlight_row = row;
	highlight_col = col;
	highlight = true;
	UpdateHighlight();
}

//returns false if position invalid
bool lifeView::FindMousePositionOnBoard(wxMouseEvent& evt, int& row, int& col) const
{
	//assumption is that board starts at 0,0 corner of panel, because the view window doesnt have anything else in it
	//and is filled with a spacer the same size as the board

	int mouseX(0), mouseY(0);
	evt.GetPosition(&mouseX, &mouseY);
	//find row and column based on the size of a single element, avoid a divide by zero error
	int tempRow = (mouseY <= 0) ? 0 : mouseY / cellsize;
	int tempCol = (mouseX <= 0) ? 0 : mouseX / cellsize;
	if(!board)
		return false;

	if (board->isValidElement(tempRow, tempCol)) 
	{
		row = tempRow;
		col = tempCol;
		return true;
	}
	else
	{
		return false;
	}
}

void lifeView::HighlightOff()
{
	highlight = false;
}
void lifeView::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC dc(this); //have to have some sort of paint dc even if not used else documentation says windows has odd behaviour
	DrawBoard(dc, true); //full redraw, usually happens on window move, minimize, resize, or when forced by a large change to the board
	evt.Skip(); //let default handling happen too
}
void lifeView::NotifyCellChanged(int row, int col, bool alive)
{
	wxClientDC dc(this);
	if(alive)
	{
		dc.SetBrush(*wxBLACK_BRUSH); //are these slow enough that'd itd be better to do two for loops of the whole thing?
		dc.SetPen(*wxGREEN_PEN);
	}
	else
	{
		dc.SetBrush(*wxLIGHT_GREY_BRUSH);
		dc.SetPen(*wxGREY_PEN);
	}
	dc.DrawRectangle(cellsize*col, cellsize*row, cellsize, cellsize);
}


void lifeView::CreateBitmapFromBoard(bool full_redraw)
{
	//get access to the bitmap as a memory dc
	wxMemoryDC dc;
	dc.SelectObject(boardBMP);
	DrawBoard(dc, full_redraw);
	dc.SelectObject(wxNullBitmap); //deselect the bitmap from the dc
}

void lifeView::DrawBoard(wxDC &dc, bool full_redraw)
{
	if(board)
	{
		wxString adj;
		int num_rows= board->getRows();
		int num_cols = board->getColumns();
		//go through each element of the board and draw a rectangle, colored based on state of element
		for(int row = 0; row < num_rows; ++row)
		{
			for(int col = 0; col < num_cols; ++col)
			{
				cell spot(board->getCell(row, col));
				if(spot.changed || full_redraw)
				{
					if(spot.living)
					{
						dc.SetBrush(*wxBLACK_BRUSH); //are these slow enough that'd itd be better to do two for loops of the whole thing?
						dc.SetPen(*wxGREEN_PEN);
					}
					else
					{
						dc.SetBrush(*wxLIGHT_GREY_BRUSH);
						dc.SetPen(*wxGREY_PEN);
					}
					dc.DrawRectangle(cellsize*col, cellsize*row, cellsize, cellsize);
				}
			}
		}
	}
	else
	{
		std::runtime_error("Board not valid in DrawBoard.");
	}
}
void lifeView::UpdateHighlight()
{
	//get access to the bitmap as a memory dc
	wxMemoryDC dc;
	dc.SelectObject(boardBMP);
	ClearHighlight(dc);
	DrawHighlight(dc);
	dc.SelectObject(wxNullBitmap); //deselect the bitmap from the dc
}
void lifeView::DrawHighlight(wxDC &dc)
{
	//draw a square around the element the mouse is hovering over
	if(highlight)
	{
		dc.SetPen(*wxCYAN_PEN);
		dc.DrawRectangle(cellsize*highlight_col, cellsize*highlight_row, cellsize, cellsize);
		prev_highlight_row = highlight_row;
		prev_highlight_col = highlight_col;
	}
}
void lifeView::ClearHighlight(wxDC &dc)
{
	//clear the old highlight
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	if(board)
	{
		(board->getCell(prev_highlight_row, prev_highlight_col).living) ? dc.SetPen(*wxGREEN_PEN) :	dc.SetPen(*wxGREY_PEN);
	}
	dc.DrawRectangle(cellsize*prev_highlight_col, cellsize*prev_highlight_row, cellsize, cellsize);
}

void lifeView::ResizeBitmap(int rows, int cols)
{
	wxSize newsize(cols*cellsize, rows*cellsize); // columns first, b/c thats width. then rows for height
	boardSizer->SetMinSize(newsize);
	boardSizer->Layout();
	boardBMP.Create(newsize);
	RedrawFull();
}

void lifeView::RedrawFull()
{
	Refresh(); //triggers a paint event
}

void lifeView::OnMouseCaptureLost(wxMouseCaptureLostEvent& evt)
{
	if(HasCapture())
		ReleaseMouse();
	evt.Skip(); //let default handling happen too
}