#include "wx/wx.h"
#include "wx/sizer.h"

#include "../header/DrawPane.h"



BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
// some useful events
/*
EVT_MOTION(BasicDrawPane::mouseMoved)
EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
EVT_LEFT_UP(BasicDrawPane::mouseReleased)
EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
EVT_KEY_DOWN(BasicDrawPane::keyPressed)
EVT_KEY_UP(BasicDrawPane::keyReleased)
EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
*/

// catch paint events
EVT_PAINT(BasicDrawPane::paintEvent)

END_EVENT_TABLE()
// some useful events
/*
void BasicDrawPane::mouseMoved(wxMouseEvent& event) {}
void BasicDrawPane::mouseDown(wxMouseEvent& event) {}
void BasicDrawPane::mouseWheelMoved(wxMouseEvent& event) {}
void BasicDrawPane::mouseReleased(wxMouseEvent& event) {}
void BasicDrawPane::rightClick(wxMouseEvent& event) {}
void BasicDrawPane::mouseLeftWindow(wxMouseEvent& event) {}
void BasicDrawPane::keyPressed(wxKeyEvent& event) {}
void BasicDrawPane::keyReleased(wxKeyEvent& event) {}
*/

BasicDrawPane::BasicDrawPane(wxFrame* parent) :
	wxPanel(parent)
{
}

void BasicDrawPane::paintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	render(dc);
}

void BasicDrawPane::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void BasicDrawPane::render(wxDC&  dc)
{
	wxImageHandler * bmpLoader = new wxBMPHandler();
	wxImage::AddHandler(bmpLoader);

	wxImage bmp("C:\\Users\\Martin\\Desktop\\Projects\\NumberRecognition\\FFTNN\\panda.bmp", wxBITMAP_TYPE_BMP);
	if(bmp.IsOk())
		dc.DrawBitmap(bmp, 0,0);
}
