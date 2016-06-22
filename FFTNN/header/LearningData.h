#pragma once
#include <wx/dir.h>
#include <vector>
#include "../header/BinaryImage.h"

class LearningData
{
	public:
		LearningData(wxString& rDirPath)
		{
			wxDir dir(rDirPath);
			if (!dir.IsOpened())
			{
				return;// TODO ...
			}


			wxString filename;
			bool cont = dir.GetFirst(&filename, "", wxDIR_FILES);
			while (cont)
			{
				dataSet.push_back(new BinaryImage(filename));
				cont = dir.GetNext(&filename);
			}
		}
	private:
		std::vector<BinaryImage*> dataSet;
};