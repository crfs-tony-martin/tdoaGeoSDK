#pragma once
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <complex>
#include <valarray>
#include <vector>
#include <iostream>
#include <fstream>
#include <mutex>
#ifdef IPP
#include <ipps.h>
#endif

#define FFT_SIZE(x) (static_cast<int>(pow(2,ceil(log2(static_cast<double>(x))))))

typedef std::valarray<std::complex<double>> TSignal;

class fft
{
public:
	fft();
	~fft(){};
	void transform(TSignal &x);
	void invert(TSignal &x);
	void power(TSignal &x, std::valarray<double> &y);
	void transform(double real[], double imag[], unsigned int n);
	void invert(double real[], double imag[], unsigned int n);
	void transform(std::vector<double> &real, std::vector<double> &imag);
	void invert(std::vector<double> &real, std::vector<double> &imag);
	void blackmanHarris(TSignal &x);
	TSignal spectrum;
	void coolytukey(TSignal &x);
private:

	unsigned int m_logN;
	unsigned int m_N;
	TSignal *m_valarray;
	TSignal twiddle;
	std::mutex m_mtx;
};


