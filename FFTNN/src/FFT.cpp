//#include "../header/FFT.h"
//#include <math.h>  
//
//using namespace std;
//
////static
//void FFT::FFT2D(Complex **c, size_t size)
//{
//	FFTbyRows(c, size);
//	FFTbyColumns(c, size);
//}
//
////static 
//void FFT::FFTbyColumns(Complex **c, size_t size)
//{
//	FFTContext context(size);
//	for (size_t i = 0; i < size; i++)
//	{
//		for (size_t j = 0; j< size; j++)
//		{
//			context.m_Real[j] = c[i][j].m_Real;
//			context.m_Imag[j] = c[i][j].m_Imag;
//		}
//		FFToperation(context);
//		for (size_t j = 0; j < size; j++)
//		{
//			c[i][j].m_Real = context.m_Real[j];
//			c[i][j].m_Imag = context.m_Imag[j];
//		}
//	}
//
//}
//
////static 
//void FFT::FFTbyRows(Complex **c, size_t size)
//{
//	FFTContext context(size);
//	for (size_t j = 0; j < size; j++)
//	{
//		for (size_t i = 0; i < size; i++)
//		{
//			context.m_Real[j] = c[i][j].m_Real;
//			context.m_Imag[j] = c[i][j].m_Imag;
//		}
//		FFToperation(context);
//		for (size_t i = 0; i < size; i++)
//		{
//			c[i][j].m_Real = context.m_Real[j];
//			c[i][j].m_Imag = context.m_Imag[j];
//		}
//	}
//}
//
////static 
//void FFT::FFTCompute( FFT::FFTContext &context)
//{
//	/* Compute the FFT */
//	size_t size = context.GetSize();
//	vector<double> &real = context.m_Real;
//	vector<double> &imag = context.m_Imag;
//	double c1 = -1.0;
//	double c2 = 0.0;
//	long l2 = 1;
//	for (size_t l = 0; l<log(size); l++) {
//		long l1 = l2;
//		l2 <<= 1;
//		double u1 = 1.0;
//		double u2 = 0.0;
//		for (size_t j = 0; j<l1; j++)
//		{
//			for (size_t i = j; i< size; i += l2)
//			{
//				size_t idx1 = i + l1;
//				double t1 = u1 * real[idx1] - u2 * imag[idx1];
//				double t2 = u1 * imag[idx1] + u2 * real[idx1];
//				real[idx1] = real[i] - t1;
//				imag[idx1] = imag[i] - t2;
//				real[i] += t1;
//				imag[i] += t2;
//			}
//			u2 = u1 * c2 + u2 * c1;
//			u1 = u1 * c1 - u2 * c2;
//		}
//		//Only forwar transfrom
//		c2 = -sqrt((1.0 - c1) / 2.0);
//
//		c1 = sqrt((1.0 + c1) / 2.0);
//	}
//
//}
//
////static 
//void FFT::ScalinForward(FFT::FFTContext &context)
//{
//	size_t size = context.GetSize();
//	for (size_t i = 0; i<size; i++)
//	{
//		context.m_Real[i] /= (double)size;
//		context.m_Imag[i] /= (double)size;
//	}
//}
//
//void  FFT::FFToperation(FFT::FFTContext &context)
//{
//	size_t  ShiftSize;
//	double tx, ty ;
//
//	size_t size = context.GetSize();
//	ShiftSize = size >> 1;
//	size_t j = 0;
//
//	vector<double> &real = context.m_Real;
//	vector<double> &imag = context.m_Imag;
//	for (size_t i = 0; i<size - 1; i++)
//	{
//		if (i < j) 
//		{
//			tx = real[i];
//			ty = imag[i];
//			real[i] = real[j];
//			imag[i] = imag[j];
//			real[j] = tx;
//			imag[j] = ty;
//		}
//		size_t k = ShiftSize;
//		while (k <= j)
//		{
//			j -= k;
//			k >>= 1;
//		}
//		j += k;
//	}
//	FFTCompute(context);
//
//}
//
//
//FFT::FFTContext::FFTContext(size_t size)
//{
//	m_Imag.resize(size);
//	m_Real.resize(size);
//}
//
//FFT::FFTContext::~FFTContext()
//{
//}
