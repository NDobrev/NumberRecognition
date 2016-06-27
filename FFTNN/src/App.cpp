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
#include "../header/Constants.h"

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
	void OnTrainNNs(wxCommandEvent& event);
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
	ID_TrainNNs = 5,
};
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_FFT, MyFrame::OnFFT)
EVT_MENU(ID_FolderFFT, MyFrame::OnFolderFFT)
EVT_MENU(ID_TrainNN, MyFrame::OnTrainNN)
EVT_MENU(ID_TrainNNs, MyFrame::OnTrainNNs)
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
	menuFile->Append(ID_TrainNNs, "&All Neural Network Training\tCtrl - N");
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

	NeuralNetwork nn(25, 150, 1);
	std::vector<DataEntry*> training;
	std::vector<DataEntry*> testing;
	TrainingSetDataReader.GenerateDataEntries(training);
	TestingSetDataReader.GenerateDataEntries(testing);
	vector<double > errorHistory;
	nn.TrainNetwork(training, testing, testing, errorHistory);
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
	BinaryImage<double> img(bmp.Scale(SIZE_FFT, SIZE_FFT));
	BinaryImage<double> *res = new BinaryImage<double>(SIZE_FFT, SIZE_FFT);
	FTFrequency::FTransform(img, *res);
	return LearningData::GetDataFromFFTImage(res);
}

struct RecognizerDescr
{
	NeuralNetwork nn;
	wxString	  name;
};

void MyFrame::OnRecognize(wxCommandEvent& event)
{
	wxDirDialog opedNNDir(NULL, "Choose NNs directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (opedNNDir.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

	wxDir dir(opedNNDir.GetPath());
	if (!dir.IsOpened())
	{
		wxLogError("Cannot open dir '%s'.", opedNNDir.GetPath());
		return;
	}
	
	wxString filename;
	bool cont = dir.GetFirst(&filename, "", wxDIR_FILES);

	vector<RecognizerDescr*> RecognizersSet;
	while (cont)
	{
		RecognizerDescr *rd = new RecognizerDescr;
		rd->nn.ReadFromFile(opedNNDir.GetPath() + "/" + filename);
		rd->name = filename;
		RecognizersSet.push_back(rd);
		cont = dir.GetNext(&filename);
	}

	wxFileDialog openFileDialog(this, _("Open image file"), "", "",
			"Image files(*.bmp) | *.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	double *input = GetNNInputByFile(openFileDialog.GetPath());
	double maxVal = 0.;
	size_t indexMaxVal = 0;
	char bufferAll[2000];
	for (size_t i = 0; i < RecognizersSet.size(); ++i)
	{
		const vector<double>& output = RecognizersSet[i]->nn.FeedInput(input);
		sprintf(bufferAll, "%s \n max appurtenance: %f for item: %s",bufferAll, output[0], RecognizersSet[i]->name.c_str().AsChar());
		if (maxVal < output[0])
		{
			indexMaxVal = i;
			maxVal = output[0];
		}
	}
	
	char buffer[500];
	sprintf(buffer, "max appurtenance: %f for item: %s", maxVal, RecognizersSet[indexMaxVal]->name.c_str().AsChar());
	wxMessageBox(bufferAll,
		"", wxOK | wxICON_INFORMATION);

	for (size_t i = 0; i < RecognizersSet.size(); ++i)
		delete RecognizersSet[i];

}

struct LearnignDataDesc
{
	LearningData* data;
	wxString      name;
};


static void GenTrainingInputData(vector<LearnignDataDesc*> &LearningSets,
								std::vector<DataEntry*> &training,
								std::vector<DataEntry*> &testing, 
								size_t index)
{
	{
		LearningData& main = *LearningSets[index]->data;
		main.GetExamples(0, main.GetSize() * 0.9, 1., training);
		main.GetExamples(main.GetSize() * 0.9, main.GetSize(), 1., testing);
	}
	for (size_t i = 0; i < LearningSets.size(); i++)
	{
		if (i != index)
		{
			LearningData& current = *LearningSets[i]->data;
			current.GetExamples(0, current.GetSize() * 0.1, 0., training);
			current.GetExamples(current.GetSize() * 0.15, current.GetSize() * 0.2, 0., testing);
		}
	}
}

static  void DisplayError(vector<double > &err)
{

}

void MyFrame::OnTrainNNs(wxCommandEvent& event)
{
	wxDirDialog opedDirTrainingSet(NULL, "Choose training set directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (opedDirTrainingSet.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

	wxDir dir(opedDirTrainingSet.GetPath());
	if (!dir.IsOpened())
	{
		wxLogError("Cannot open dir '%s'.", opedDirTrainingSet.GetPath());
		return;
	}

	wxString dirName;
	bool cont = dir.GetFirst(&dirName, "", wxDIR_DIRS);

	vector<LearnignDataDesc*> LearningSets;
	while (cont)
	{

		LearningSets.push_back(new LearnignDataDesc{ new LearningData(opedDirTrainingSet.GetPath()+ "/" + dirName), dirName });
		cont = dir.GetNext(&dirName);
	}
	
	#pragma omp parallel
	#pragma omp for
	for (short i = 0; i < LearningSets.size(); ++i)
	{
		std::vector<DataEntry*> training;
		std::vector<DataEntry*> testing;
		GenTrainingInputData(LearningSets, training, testing, i);
		NeuralNetwork nn(_INPUT_NN_SIZE*_INPUT_NN_SIZE, 3* _INPUT_NN_SIZE*_INPUT_NN_SIZE, 1);
		nn.SetMaxEpochs(50000);
		nn.SetDesiredAccuracy(90);
		nn.SetLearningParameters(0.0001, 0.8);
		vector<double > trainingErrHistory;
		nn.TrainNetwork(training, testing, testing, trainingErrHistory);
		nn.WriteToFile(opedDirTrainingSet.GetPath() + "/" + LearningSets[i]->name + ".nn");
	}
}
