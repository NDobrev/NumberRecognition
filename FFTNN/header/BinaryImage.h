#pragma once
#include <wx\image.h>

class BinaryImage
{
	public:
	BinaryImage(short h, short w)
		:
		mH(h),
		mW(w),
		mData(new unsigned char[h, w])
	{
	}

	BinaryImage(wxImage &rBmp)
	{
		mW = rBmp.GetWidth();
		mH = rBmp.GetHeight();
		for (size_t i; i < mH; ++i)
			for (size_t j; j < mW; ++j)
			{
				mData[i * mW + j] = ((0.299 * rBmp.GetRed(i, j) + 0.587 * rBmp.GetGreen(i, j) + 0.114 * rBmp.GetBlue(i, j)) * 255);
			}
	}

	void Set(short h, short w, unsigned char intensity)
	{
		mData[mW * h + w] = intensity;
	}

	unsigned char  Get(short h, short w)
	{
		return mData[mW *h + w];
	}

	const unsigned char * GetData()
	{
		return mData;
	}
	private:
		short mW;
		short mH;
		unsigned char *mData;
};