#pragma once
#include <wx\image.h>
#include "../header/Complex.h"
class BinaryImage
{
	public:
	BinaryImage(short h, short w)
		:
		mH(h),
		mW(w),
		mData(new unsigned char[h * w])
	{
	}

	BinaryImage(wxImage &rBmp)
	{
		mW = rBmp.GetWidth();
		mH = rBmp.GetHeight();
		mData = new unsigned char[mW * mH];
		for (short i = 0 ; i < mH; ++i)
			for (short j = 0; j < mW; ++j)
			{
				float data = (0.299 * (float)rBmp.GetRed(j, i) + 0.587 * (float)rBmp.GetGreen(j, i) + 0.114 * (float)rBmp.GetBlue(j, i));
				mData[i * mW + j] = data ;
			}
	}

	~BinaryImage()
	{
		//delete []mData;
	}

	bool IsSquare() const
	{
		return mW == mH;
	}

	void Set(short h, short w, unsigned char intensity)
	{
		mData[mW * h + w] = intensity;
	}

	unsigned char  Get(short h, short w) const
	{
		return mData[mW *h + w];
	}

	const unsigned char * GetData() const
	{
		return mData;
	}

	// return ref to rResult 
	wxImage& GetAsImage(wxImage &rResult)
	{
		rResult.Rescale(mW, mH);

		for (short i = 0; i < mH; ++i)
			for (short j = 0; j < mW; ++j)
			{
				unsigned char data = mData[i * mW + j];
				rResult.SetRGB(j, i,data, data, data);
			}
		return rResult;
	}

	Complex** GetAsComplex() const
	{
		Complex **result;
		result = new Complex*[mW];
		for (short i = 0; i < mH; ++i)
		{
			result[i] = new Complex[mW];
			for (short j = 0; j < mW; ++j)
			{
				result[i][j].m_Imag = 0.f;
				result[i][j].m_Real = mData[i * mW + j];
			}
		}
		return result;
	}

	short GetW() const
	{
		return mW;
	}

	short GetH() const 
	{
		return mH;
	}

	private:
		short mW;
		short mH;
		unsigned char *mData;
};