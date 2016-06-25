#include "../header/LearningData.h"


LearningData::LearningData(wxString& rDirPath)
{
	LoadExample(rDirPath, true);
	LoadExample(rDirPath + "/Neg", false);
}

LearningData::~LearningData()
{
	for (DataSetIter it = mDataSet.begin(); it != mDataSet.end(); ++it)
		delete *it;
}

void LearningData::LoadExample(wxString& rDirPath, bool bPositive)
{
	wxDir dir(rDirPath);
	if (!dir.IsOpened())
	{
		wxLogError("Cannot open dir '%s'.", rDirPath);
		return;
	}

	wxString filename;
	bool cont = dir.GetFirst(&filename, "", wxDIR_FILES);
	wxImageHandler * bmpLoader = new wxBMPHandler();
	wxImage::AddHandler(bmpLoader);

	while (cont)
	{

		wxImage bmp(rDirPath + "/" + filename, wxBITMAP_TYPE_BMP);
		if (bmp.IsOk())
		{

			BinaryImage img(bmp.Scale(25, 25));
			BinaryImage *res = new BinaryImage(25, 25);
			FTFrequency::FTransform(img, *res);
			mDataSet.push_back(new Entry{ bPositive, res  });
		}
		cont = dir.GetNext(&filename);
	}
}



void LearningData::SaveInDir(wxString& rDirPath)
{
	for (size_t i = 0; i < mDataSet.size(); ++i)
	{
		if (mDataSet[i]->mbPositive)
		{
			wxImage image(wxSize(mDataSet[i]->mExample->GetSize()));
			mDataSet[i]->mExample->GetAsImage(image);
			char buffer[512];
			sprintf(buffer, "%s/FFT/%i.bmp", rDirPath.char_str().data(), i);
			wxMkDir(rDirPath + "/" + "FFT");
			image.SaveFile(buffer);
		}
	}
}

std::vector<DataEntry *>& LearningData::GenerateDataEntries(std::vector<DataEntry *> &rResult)
{
	rResult.resize(mDataSet.size());
	for (size_t it = 0; it < mDataSet.size(); ++it)
	{

		double *pInput = GetDataFromFFTImage(mDataSet[it]->mExample);
		double *pOutput = new double[1];
		pOutput[0] = mDataSet[it]->mbPositive ? 1. : 0.;
		rResult[it] = new DataEntry(pInput, pOutput);
	}
	return rResult;
}

double * LearningData::GetDataFromFFTImage(const BinaryImage *res)
{
	// 5 x 5 sq 
	double *pInput = new double[25];
	for (short i = 0; i < 5; ++i)
		for (short j = 0; j < 5; j++)
		{
			pInput[i * 5 + j] = res->Get(10 + i, 10 + j);
		}
	return pInput;
}
