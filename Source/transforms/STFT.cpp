/*
  ==============================================================================

    STFT.cpp
    Created: 21 May 2015 2:29:08pm
    Author:  R00070693

  ==============================================================================
*/

#include "STFT.h"
#include "JuceHeader.h"
#include <iostream>

STFT::STFT(int size) : fft(size)
{

}

STFT::~STFT()
{
	// destructor
}

float* STFT::performForwardTransform(const float* array)
{
	float* results = new float[WINDOW_SIZE];
	results[0] = {};
	
	fft.do_fft(results, array);

	return results;

}

// initialises and assigns window array
void STFT::initWindow(int windowType)
{
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

std::complex<float>* STFT::realToComplex(float* data, int size)
{	
	numSamples = size;
	int complexSize = (size / 2);
	std::complex<float>* comp = new std::complex<float>[complexSize];

	for (int i = 0; i < complexSize; i++)
	{
		if (i == 0)
		{
			comp[i] = { data[i], 0.0f };
		}
		else if (i == complexSize)
		{
			comp[i] = { data[i], 0.0f };
		}
		else
		{
			comp[i] = { data[i], data[i + complexSize] };
		}
	}

	return comp;
}

