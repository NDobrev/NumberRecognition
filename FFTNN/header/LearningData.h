#pragma once
#include <wx/dir.h>
#include <wx/log.h>
#include <vector>
#include "../header/BinaryImage.h"
#include "../header/FFTMagnitude.h"
#include "../header/DataEntry.h"

class LearningData
{
	public:
		struct Entry
		{
			bool mbPositive;
			BinaryImage<double>* mExample;

			~Entry()
			{
				delete mExample;
			}
		};
		typedef std::vector<Entry*> DataSet;
		typedef std::vector<Entry*>::const_iterator DataSetConstIter;
		typedef std::vector<Entry*>::iterator DataSetIter;

		//return new double arr 
		static double * GetDataFromFFTImage(const BinaryImage<double> *res);
		LearningData(wxString& rDirPath);
		~LearningData();
		void GetExamples(size_t from, size_t to, double desireOutput,std::vector<DataEntry *> &rResult);
		void LoadExample(wxString& rDirPath, bool bPositive);
		void SaveInDir(wxString& rDirPath);
		std::vector<DataEntry *>& GenerateDataEntries(std::vector<DataEntry *> &rResult);
		size_t GetSize() { return mDataSet.size(); }
		DataSetConstIter GetBeg() { mDataSet.begin(); }
		DataSetConstIter GetEnd() { mDataSet.end(); }
	private:
		DataSet mDataSet;
};