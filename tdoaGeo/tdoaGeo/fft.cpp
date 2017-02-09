// fft.cpp : Defines the exported functions for the DLL application.
//
#include "fft.h"
#include <algorithm>

using namespace std;

fft::fft()
{
	twiddle.resize(2);
}

void fft::blackmanHarris(TSignal &x)
{
    double N = static_cast<double>(x.size());
    for (size_t i = 0; i < x.size() / 2; i++)
    {
        double window = 0.35875
                    - (0.48829 * cos(2 * M_PI * static_cast<double>(i) / (N - 1)))
                    + (0.14128 * cos(4 * M_PI * static_cast<double>(i) / (N - 1)))
                    - (0.01168 * cos(6 * M_PI * static_cast<double>(i) / (N - 1)));
		x[i] *= window;
		x[x.size() - 1 - i] *= window;
     }
}

void fft::power(TSignal &x, valarray<double> &y)
{
	if(x.size() != y.size())
	{
		y.resize(x.size());
	}

	for(size_t i = 0;i < x.size();i++)
	{
		y[i] = std::abs(x[i]);
	}
}

void fft::transform(double real[], double imag[], unsigned int n)
{
	try
	{
		unsigned int m_N = 1;
		for (m_logN = 0; m_N < n; m_logN++)
		{
			m_N *= 2;
		}
		m_valarray = new valarray<complex<double> >(m_N);
		for(unsigned int i = 0;i < n;i++)
		{
			m_valarray[i] = complex<double>(real[i], imag[i]);
		}
		transform(*m_valarray);
		for(int unsigned i = 0;i < n;i++)
		{
			real[i] = (*m_valarray)[i].real();
			imag[i] = (*m_valarray)[i].imag();
		}
		delete m_valarray;
	}
	catch(...)
	{

	}
}

void fft::invert(double real[], double imag[], unsigned int n)
{
	try
	{
		unsigned int m_N = 1;
		for (m_logN = 0; m_N < n; m_logN++)
		{
			m_N *= 2;
		}
		m_valarray = new valarray<complex<double> >(m_N);
		for(unsigned int i = 0;i < n;i++)
		{
			m_valarray[i] = complex<double>(real[i], imag[i]);
		}
		invert(*m_valarray);
		for(unsigned int i = 0;i < n;i++)
		{
			real[i] = (*m_valarray)[i].real();
			imag[i] = (*m_valarray)[i].imag();
		}
		delete m_valarray;
	}
	catch(...)
	{

	}
}

void fft::transform(std::vector<double> &real, std::vector<double> &imag)
{
	try
	{
		size_t n = real.size();
		unsigned int m_N = 1;
		for (m_logN = 0; m_N < n; m_logN++)
		{
			m_N *= 2;
		}
		TSignal s(m_N);
		for (size_t i = 0; i < n; i++)
		{
			s[i] = complex<double>(real[i], imag[i]);
		}
		transform(s);
		for(size_t i = 0; i < n; i++)
		{
			real[i] = s[i].real();
			imag[i] = s[i].imag();
		}
	}
	catch (...)
	{

	}
}

void fft::invert(std::vector<double> &real, std::vector<double> &imag)
{
	try
	{
		size_t n = real.size();
		unsigned int m_N = 1;
		for (m_logN = 0; m_N < n; m_logN++)
		{
			m_N *= 2;
		}
		TSignal s(m_N);
		for (size_t i = 0; i < n; i++)
		{
			s[i] = complex<double>(real[i], imag[i]);
		}
		invert(s);
		for (size_t i = 0; i < n; i++)
		{
			real[i] = s[i].real();
			imag[i] = s[i].imag();
		}
	}
	catch (...)
	{

	}
}

void fft::transform(TSignal &x)
{
	//std::lock_guard<std::mutex> lk(m_mtx);
	coolytukey(x);
}

#ifndef IPP
// Cooley–Tukey FFT (in-place)
void fft::coolytukey(TSignal &x)
{
    const size_t N = x.size();
    if(N/2 > twiddle.size())
    {
	    //cout << "computing twiddle" << endl;
	    //recompute twiddle factors
	    twiddle.resize(N/2);
	    for(size_t k = 0; k < N/2; ++k)
	    {
		    twiddle[k] = std::polar<double>(1.0, -2 * M_PI * k / N);
	    }
    }
    if (N <= 1)
    {
	    return;
    }
    // divide
    valarray<complex<double>> even = x[std::slice(0, N/2, 2)];
    valarray<complex<double>>  odd = x[std::slice(1, N/2, 2)];
 
	valarray<complex<double>>  *p1 = &x;
	valarray<complex<double>>  *p2 = &even;

    // conquer
    transform(even);
    transform(odd);
 
    // combine
	size_t step = 2 * twiddle.size() / N;
    for (size_t i = 0, k = 0; k < N/2; ++k, i += step)
    {
		complex<double> t = twiddle[i] * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}

#else

void fft::coolytukey(TSignal &x)
{
	const int N = x.size();

	// Spec and working buffers
	IppsDFTSpec_C_32fc *pDFTSpec = 0;
	Ipp8u  *pDFTInitBuf, *pDFTWorkBuf;

	// Allocate complex buffers
	Ipp32fc *pSrc = ippsMalloc_32fc(N);
	for (size_t i = 0; i < N; i++)
	{
		pSrc[i].re = x[i].real();
		pSrc[i].im = x[i].imag();
	}
	
	Ipp32fc *pDst = ippsMalloc_32fc(N);

	// Query to get buffer sizes
	int sizeDFTSpec, sizeDFTInitBuf, sizeDFTWorkBuf;

	ippsDFTGetSize_C_32fc(N, IPP_FFT_NODIV_BY_ANY,
	ippAlgHintAccurate, &sizeDFTSpec, &sizeDFTInitBuf, &sizeDFTWorkBuf);

	// Alloc DFT buffers
	pDFTSpec = (IppsDFTSpec_C_32fc*)ippsMalloc_8u(sizeDFTSpec);
	pDFTInitBuf = ippsMalloc_8u(sizeDFTInitBuf);
	pDFTWorkBuf = ippsMalloc_8u(sizeDFTWorkBuf);

	// Initialize DFT
	ippsDFTInit_C_32fc(N, IPP_FFT_NODIV_BY_ANY,	ippAlgHintAccurate, pDFTSpec, pDFTInitBuf);
	if (pDFTInitBuf) ippsFree(pDFTInitBuf);
	// Do the DFT
	ippsDFTFwd_CToC_32fc(pSrc, pDst, pDFTSpec, pDFTWorkBuf);

	if (pDFTWorkBuf) ippsFree(pDFTWorkBuf);
	if (pDFTSpec) ippsFree(pDFTSpec);
	ippsFree(pSrc);
	for (size_t i = 0; i < N; i++)
	{
		x[i] = std::complex<double>(pDst[i].re, pDst[i].im);
	}
	ippsFree(pDst);
}
#endif

// inverse fft (in-place)
void fft::invert(TSignal &x)
{
    // conjugate the complex numbers
	x = x.apply(std::conj);
	//for(size_t i = 0;i < x.size();i++)
	//	x[i] = std::conj(x[i]);

    // forward fft
    transform(x);
 
    // conjugate the complex numbers again
	x = x.apply(std::conj);
	//for(size_t i = 0;i < x.size();i++)
	//	x[i] = std::conj(x[i]);
    //scale the numbers
    x /= static_cast<double>(x.size());
}


