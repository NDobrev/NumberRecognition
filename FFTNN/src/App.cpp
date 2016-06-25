// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/filepicker.h>

#include "../header/DrawPane.h"
#include "../header/LearningData.h"
#include "../header/NN.h"

class MyApp : public wxApp
{
	public:
		virtual bool OnInit();
};


class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	void OnFFT(wxCommandEvent& event);
	void OnFolderFFT(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnTrainNN(wxCommandEvent& event);
	void OnRecognize(wxCommandEvent& event);
private:
	BasicDrawPane * pCanvas;
	wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_FFT = 1,
	ID_FolderFFT = 2,
	ID_TrainNN = 3,
	ID_Recognize = 4,
};
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_FFT, MyFrame::OnFFT)
EVT_MENU(ID_FolderFFT, MyFrame::OnFolderFFT)
EVT_MENU(ID_TrainNN, MyFrame::OnTrainNN)
EVT_MENU(ID_Recognize, MyFrame::OnRecognize)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyFrame *frame = new MyFrame("FFTNN", wxPoint(50, 50), wxSize(640, 480));
	frame->Show(true);
	return true;
}
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
	, pCanvas(new BasicDrawPane(this))
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(pCanvas);
	SetSizer(sizer);
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_FFT, "&Fourier transform\tCtrl-F");
	menuFile->Append(ID_FolderFFT, "&Folder Fourier transform\tCtrl-O");
	menuFile->Append(ID_TrainNN, "&Neural Network Training\tCtrl-T");
	menuFile->Append(ID_Recognize, "&Recognize \tCtrl-R");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
	CreateStatusBar();
}
void MyFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("",
		"About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnFolderFFT(wxCommandEvent& event)
{
	wxDirDialog opedDirDialog(NULL, "Choose input directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (opedDirDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

					// proceed loading the file chosen by the user;
					// this can be done with e.g. wxWidgets input streams:
	LearningData set(opedDirDialog.GetPath());
	set.SaveInDir(opedDirDialog.GetPath());
	wxMessageBox("Done",
		"", wxOK | wxICON_INFORMATION);
}


void MyFrame::OnFFT(wxCommandEvent& event)
{

	wxFileDialog
		openFileDialog(this, _("Open image file"), "", "",
			"Image files(*.bmp) | *.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

					// proceed loading the file chosen by the user;
					// this can be done with e.g. wxWidgets input streams:
	
	if(!pCanvas->OnFFT(openFileDialog.GetPath()))
	{

		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}
}

void MyFrame::OnTrainNN(wxCommandEvent& event)
{
	wxDirDialog opedDirTrainingSet(NULL, "Choose training set directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (opedDirTrainingSet.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...
	LearningData TrainingSetDataReader(opedDirTrainingSet.GetPath());

	// 
	wxDirDialog opedDirTestingSet(NULL, "Choose training set directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (opedDirTestingSet.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

	LearningData TestingSetDataReader(opedDirTestingSet.GetPath());

	NeuralNetwork nn(25, 50, 1);
	std::vector<DataEntry*> training;
	std::vector<DataEntry*> testing;
	TrainingSetDataReader.GenerateDataEntries(training);
	TestingSetDataReader.GenerateDataEntries(testing);
	nn.TrainNetwork(training, testing, testing);
	wxFileDialog	openNNFileDialog(this, _("Neural net files"), "", "",
		"Neural net files(*.nn) | *.nn", wxFD_OPEN );
	if (openNNFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

	nn.WriteToFile(openNNFileDialog.GetPath());
}

static inline double* GetNNInputByFile(wxString file)
{
	wxImageHandler * bmpLoader = new wxBMPHandler();
	wxImage::AddHandler(bmpLoader);
	wxImage bmp(file, wxBITMAP_TYPE_BMP);
	BinaryImage img(bmp.Scale(25, 25));
	BinaryImage *res = new BinaryImage(25, 25);
	FTFrequency::FTransform(img, *res);
	return LearningData::GetDataFromFFTImage(res);
}

void MyFrame::OnRecognize(wxCommandEvent& event)
{
	wxFileDialog	openNNFileDialog(this, _("Neural net files"), "", "",
		"Neural net files(*.nn) | *.nn", wxFD_OPEN);
	if (openNNFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...
	NeuralNetwork nn;
	nn.ReadFromFile(openNNFileDialog.GetPath());



	wxFileDialog openFileDialog(this, _("Open image file"), "", "",
			"Image files(*.bmp) | *.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	double *input = GetNNInputByFile(openFileDialog.GetPath());
	double *output = nn.FeedInput(input);
	if(output[0] < 0.5)
		wxMessageBox("Not recognize",
			"", wxOK | wxICON_INFORMATION);
	else
		wxMessageBox("Nice",
			"", wxOK | wxICON_INFORMATION);

}