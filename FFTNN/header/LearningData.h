#pragma once
#include <wx/dir.h>
#include <wx/log.h>
#include <vector>
#include "../header/BinaryImage.h"

class LearningData
{
	public:
		typedef std::vector<BinaryImage*> DataSet;
		typedef std::vector<BinaryImage*>::const_iterator DataSetConstIter;
		typedef std::vector<BinaryImage*>::iterator DataSetIter;
		LearningData(wxString& rDirPath)
		{
			wxDir dir(rDirPath);
			if (!dir.IsOpened())
			{
				wxLogError("Cannot open dir '%s'.", rDirPath);
				return;
			}

			wxString filename;
			bool cont = dir.GetFirst(&filename, "", wxDIR_FILES);
			while (cont)
			{
				wxImageHandler * bmpLoader = new wxBMPHandler();
				wxImage::AddHandler(bmpLoader);
				wxImage bmp(filename, wxBITMAP_TYPE_BMP);
				if (bmp.IsOk())
					dataSet.push_back(new BinaryImage(bmp));
				cont = dir.GetNext(&filename);
			}
		}

		LearningData()
		{
			for (DataSetIter it = dataSet.begin(); it != dataSet.end(); ++it)
				delete *it;
		}

		DataSetConstIter GetBeg() { dataSet.begin(); }
		DataSetConstIter GetEnd() { dataSet.end(); }
	private:
		DataSet dataSet;
};