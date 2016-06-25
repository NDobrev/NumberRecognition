#pragma once
#include "vector"
//
//#include "../header/Complex.h"
//class FFT
//{
//	public:
//		struct FFTContext
//		{
//			std::vector<double> m_Imag;
//			std::vector<double> m_Real;
//			size_t GetSize() { return m_Imag.size(); }
//			FFTContext(size_t size);
//			~FFTContext();
//		private:
//			FFTContext(const FFTContext&);
//				FFTContext& operator=(const FFTContext&);
//		};
//
//	public:
//		static void FFT2D(Complex **c, size_t size);
//	private: 
//		static void FFTbyColumns(Complex **c, size_t size);
//		static void FFTbyRows(Complex **c, size_t size);
//		static void	FFToperation(FFTContext & context);
//		static void FFTCompute(FFTContext & context);
//		static void ScalinForward(FFTContext & context);
//
//};