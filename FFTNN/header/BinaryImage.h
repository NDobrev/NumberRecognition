#pragma once
#include <wx\image.h>
template<typename T>
class BinaryImage
{
	public:
	BinaryImage(short h, short w)
		:
		mH(h),
		mW(w),
		mData(new T[h * w])
	{
	}

	BinaryImage(wxImage &rBmp)
	{
		mW = rBmp.GetWidth();
		mH = rBmp.GetHeight();
		mData = new T[mW * mH];
		for (short i = 0 ; i < mH; ++i)
			for (short j = 0; j < mW; ++j)
			{
				float data = (0.299 * (float)rBmp.GetRed(j, i) + 0.587 * (float)rBmp.GetGreen(j, i) + 0.114 * (float)rBmp.GetBlue(j, i));
				assert(i * mW + j < mW * mH);
				mData[i * mW + j] = data ;
			}
	}

	~BinaryImage()
	{
		delete []mData;
	}

	bool IsSquare() const
	{
		return mW == mH;
	}

	void Set(short h, short w, T intensity)
	{
		assert(mW *h + w < mW * mH);
		mData[mW * h + w] = intensity;
	}

	T  Get(short h, short w) const
	{
		assert(mW *h + w < mW * mH);
		return mData[mW *h + w];
	}

	const T * GetData() const
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
				assert(i * mW + j < mW * mH);
				T data = mData[i * mW + j];
				rResult.SetRGB(j, i,data, data, data);
			}
		return rResult;
	}

	wxSize GetSize() const { return wxSize(mW, mH); }

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
		T *mData;
};