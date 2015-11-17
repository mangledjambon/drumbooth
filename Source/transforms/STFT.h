/*
  ==============================================================================

    STFT.h
    Created: 21 May 2015 2:24:47pm
    Author:  Sean Breen
	
	Handles performing a forward FFT on an array of real floats

  ==============================================================================
*/

#ifndef STFT_H_INCLUDED
#define STFT_H_INCLUDED
#include <complex>
#include "../ffft/FFTReal.h"
#include "../Constants.h"

class STFT
{

	enum windowType
	{
		HANN = 1
	};

public:

	/*
		Constructor
		input: number of samples in window (WINDOW_SIZE)
	*/
	STFT();
	~STFT();

	/*
		Performs forward FFT on float array
		Input: pointer to array of floats from wav file
		Output: pointer to transformed array of floats
	*/
	float* performForwardTransform(float* array);

	/*
		Initializes the window function
		Input: type of window; see windowType enum
		Output: none
	*/
	void initWindow(int type);

	/*
		Applies the window function to an array of real floats
		Input: array on which the fuction will be applied
		Output: none
	*/
	void applyWindowFunction(float* array);

	/*
		Converts array of real floats to complex floats
		input: pointer to array of Real floats
		Output: pointer to array converted to complex floats
	*/
	std::complex<float>* realToComplex(float* data);

	ffft::FFTReal<float> fft;
	
private:

	// private variables/methods go here
	int numSamples;
	float window[WINDOW_SIZE];
	float results[WINDOW_SIZE];
	std::complex<float> comp[WINDOW_SIZE / 2];

};



#endif  // STFT_H_INCLUDED
