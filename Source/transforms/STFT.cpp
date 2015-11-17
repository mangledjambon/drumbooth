/*
  ==============================================================================

    STFT.cpp
    Created: 21 May 2015 2:29:08pm
    Author:  R00070693

  ==============================================================================
*/

#include "STFT.h"

STFT::STFT() : fft((long)WINDOW_SIZE)
{
}

STFT::~STFT()
{
	// destructor
}

float* STFT::performForwardTransform(float* array)
{
	results[0] = {};
	
	fft.do_fft(results, array);

	return results;

}

// initialises and assigns window array
void STFT::initWindow(int windowType){

	if (windowType == HANN)
	{
		for (int i = 0; i < WINDOW_SIZE; i++){
			window[i] = 0.0f;
			window[i] = (float)(0.5*(1 - cos((TPI*i) / (WINDOW_SIZE - 1))));
		}
	}
}

void STFT::applyWindowFunction(float* array)
{
	for (int i = 0; i < WINDOW_SIZE; i++)
	{
		array[i] *= window[i];
	}
}

std::complex<float>* STFT::realToComplex(float* data)
{	
	for (int i = 0; i < (WINDOW_SIZE / 2); i++)
	{
		if (i == 0)
		{
			comp[i] = { data[i], 0.0f };
		}
		else if (i == (WINDOW_SIZE / 2))
		{
			comp[i] = { data[i], 0.0f };
		}
		else
		{
			comp[i] = { data[i], data[i + WINDOW_SIZE / 2] };
		}
	}

	return comp;
}

