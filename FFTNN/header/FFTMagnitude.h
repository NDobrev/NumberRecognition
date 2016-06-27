#pragma once
#include <complex>
#include <cmath>
#include "../header/BinaryImage.h"
#include "../header/Constants.h"

using namespace std::complex_literals;
class FTFrequency
{
public:
	// output onlt Frequency
	static BinaryImage& FTransform(const BinaryImage& image, BinaryImage& result)
	{
		assert(image.IsSquare() && result.GetH() == image.GetH());
		assert(result.GetH() == SIZE_FFT);
		short N = image.GetH();
		double normalizer = 1.f / N;
		//actualy must by 25x25
		double arr[SIZE_FFT][SIZE_FFT];
		double maxMagnitude = 0;
		for (short k = 0; k < N; ++k)
			#pragma omp parallel
			#pragma omp for
			for (short l = 0; l < N; ++l)
			{
				std::complex<double> sum = 0;
				for (short i = 0; i < N; ++i)
					for (short j = 0; j < N; ++j)
					{
						double freq = double(image.Get(i, j));
						double angle = PI2 * (double(k * i + l *j) / double(N));
						sum += freq * std::exp(-1i *angle);
					}
				// abs from complex number return magnitude 
				sum *= normalizer;
				double sq = abs(sum);
				if (maxMagnitude < sq)
					maxMagnitude = sq;
				// set (0,0) of image in center
				arr[(k + N / 2) % N][(l + N / 2) % N] = sq;
			}
		double coef = 255. / log10(1 + fabs(maxMagnitude*  0.01));
		for (short i = 0; i < SIZE_FFT; ++i)
			for (short j = 0; j < SIZE_FFT; ++j)
			{
				double lg = coef * log10(arr[i][j] * 0.01);
				lg = lg < 0 ? 0. : lg;
				result.Set(i, j, lg);
			}
		return result;
	}

};

