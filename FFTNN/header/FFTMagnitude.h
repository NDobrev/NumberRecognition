#pragma once
#include <complex>
#include <cmath>
#include "../header/BinaryImage.h"

#define PI2 6.28318530718f

using namespace std::complex_literals;
class FTFrequency
{
public:
	// output onlt Frequency
	static BinaryImage FTransform(const BinaryImage& image)
	{
		BinaryImage result(image.GetH(), image.GetW());
		assert(image.IsSquare());
		short N = image.GetH();
		double normalizer = 1.f / N;
		//actualy must by 64x64
		double arr[64][64];
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
		double coef = 255. / log10(1 + fabs(maxMagnitude*  0.15));
		for (short i = 0; i < 64; ++i)
			for (short j = 0; j < 64; ++j)
			{
				double lg = coef * log10(arr[i][j] * 0.15);
				lg = lg < 0 ? 0. : lg;
				result.Set(i, j, lg);
			}
		return result;
	}

};

