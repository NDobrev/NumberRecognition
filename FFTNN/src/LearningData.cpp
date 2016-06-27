#include "../header/FFTMagnitude.h"
#include "../header/LearningData.h"
#include "../header/Constants.h"

LearningData::LearningData(wxString& rDirPath)
{
	LoadExample(rDirPath, true);
//	LoadExample(rDirPath + "/Neg", false);
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

			BinaryImage img(bmp.Scale(SIZE_FFT, SIZE_FFT));
			BinaryImage *res = new BinaryImage(SIZE_FFT, SIZE_FFT);
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

void LearningData::GetExamples(size_t from, size_t to, double desireOutput, std::vector<DataEntry *> &rResult)
{
	assert(from < to && to <= mDataSet.size());
	for (; from < to; ++from)
	{
		double *pInput = GetDataFromFFTImage(mDataSet[from]->mExample);
		double *pOutput = new double[1];
		pOutput[0] = desireOutput;
		rResult.push_back(new DataEntry(pInput, pOutput));
	}
}

double * LearningData::GetDataFromFFTImage(const BinaryImage *res)
{
	//sq 
	double *pInput = new double[_INPUT_NN_SIZE*_INPUT_NN_SIZE];
	for (short i = 0; i < _INPUT_NN_SIZE; ++i)
		for (short j = 0; j < _INPUT_NN_SIZE; j++)
		{
			pInput[i * _INPUT_NN_SIZE + j] = res->Get((SIZE_FFT - _INPUT_NN_SIZE) /2 + i,
														(SIZE_FFT - _INPUT_NN_SIZE) / 2 + j);
		}
	return pInput;
}

