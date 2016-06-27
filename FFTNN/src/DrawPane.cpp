#include "wx/wx.h"
#include "wx/sizer.h"

#include "../header/DrawPane.h"
#include "../header/BinaryImage.h"
#include "../header/FFTMagnitude.h"
#include "../header/FFTMagnitude.h"

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

BasicDrawPane::BasicDrawPane(wxWindow* parent) :
	wxPanel(parent,-1, wxDefaultPosition, wxSize(300, 300), wxBORDER_SIMPLE)
{
}

BasicDrawPane::~BasicDrawPane()
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
	if (mBmp.IsOk())
	{
		dc.DrawBitmap(mBmp, 0, 0);
	}
}


bool BasicDrawPane::OnFFT(wxString &rPath)
{

	wxImageHandler * bmpLoader = new wxBMPHandler();
	wxImage::AddHandler(bmpLoader);
	mBmp.LoadFile(rPath, wxBITMAP_TYPE_BMP);
	BinaryImage<unsigned char> img(mBmp.Scale(SIZE_FFT, SIZE_FFT));
	BinaryImage<unsigned char> res(mBmp.Scale(SIZE_FFT, SIZE_FFT));
	mBmp = FTFrequency::FTransform<unsigned char>(img, res).GetAsImage(mBmp).Scale(300, 300);
	paintNow();
	//delete bmpLoader;
	return mBmp.IsOk();
}