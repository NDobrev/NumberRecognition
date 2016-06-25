#pragma once
#include "wx/wx.h"


class BasicDrawPane : public wxPanel
{

	public:
		BasicDrawPane(wxWindow* parent);
		~BasicDrawPane();
		void paintEvent(wxPaintEvent & evt);
		void paintNow();

		void render(wxDC& dc);
		bool OnFFT(wxString &rPath);
	private:
		wxImage mBmp;
		DECLARE_EVENT_TABLE()
};