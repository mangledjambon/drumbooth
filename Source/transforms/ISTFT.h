/*
  ==============================================================================

    ISTFT.h
    Created: 21 May 2015 2:25:12pm
    Author:  R00070693

	handles inverse FFT of complex float array

  ==============================================================================
*/
#ifndef ISTFT_H_INCLUDED
#define ISTFT_H_INCLUDED

#include "../Constants.h"
#include "../ffft/FFTReal.h"
#include "../ffft/def.h"
#include "../Eigen/Eigen"
#include <complex>

using Eigen::MatrixXcf;
using Eigen::MatrixXf;

class ISTFT
{

	enum windowType
	{
		HANN = 1
	};

public:

	/*
		Constructor
	*/
	ISTFT();

	~ISTFT();

	/*
	Initializes the window function
	Input: type of window; see windowType enum
	Output: none
	*/
	void initWindow(int type);

	/*
		Performs an in-place rescale of an array of transformed float data.
		Can be used after inverse FFT to correct scale.
		input: the array of real floats to be scaled
		output: none
	*/
	void rescale(float* arrayToScale);

	/*
		Performs inverse FFT on a column of the spectrogram
		input: column from spectrogram, destination array
		output: none
	*/
	void performInverseTransform(float* column, float* newArray);

	/*
		Converts complex matrix to real matrix
		input: Eigen complex matrix to be converted
		output: Eigen real matrix
	*/
	MatrixXf complexToReal(MatrixXcf complex);

	float window[WINDOW_SIZE];

private:

	ffft::FFTReal<float> ifft;
	Eigen::MatrixXf real;

};



#endif  // ISTFT_H_INCLUDED
