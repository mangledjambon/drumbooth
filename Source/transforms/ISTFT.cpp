/*
  ==============================================================================

    ISTFT.cpp
    Created: 21 May 2015 2:28:33pm
    Author:  R00070693

  ==============================================================================
*/

#include "ISTFT.h"

ISTFT::ISTFT() : ifft((long)WINDOW_SIZE)
{

}

ISTFT::~ISTFT()
{

}

// initialises and assigns window array
void ISTFT::initWindow(int windowType){

	if (windowType == HANN)
	{
		for (int i = 0; i < WINDOW_SIZE; i++){
			window[i] = 0.0f;
			window[i] = (float)(0.5*(1 - cos((TPI*i) / (WINDOW_SIZE - 1))));
		}
	}
}

// scales an array, should be called after IFFT
void ISTFT::rescale(float* transformedArray){
	ifft.rescale(transformedArray);
}

// performs inverse fast fourier transform on a column of the frequency matrix
void ISTFT::performInverseTransform(float* column, float* newArray)
{
	ifft.do_ifft(column, newArray);
}

MatrixXf ISTFT::complexToReal(MatrixXcf complex)
{
	MatrixXf real = MatrixXf::Zero(WINDOW_SIZE, complex.cols());

	for (int k = 0; k < complex.cols(); k++)
	{
		for (int i = 0; i < (WINDOW_SIZE / 2); i++)
		{
			if (i == 0)
			{
				real(i, k) = complex(i, k).real();
			}
			else if (i == (WINDOW_SIZE / 2))
			{			
				real(i, k) = complex(i, k).real();
			}
			else
			{
				real(i, k) = complex(i, k).real();
				real(i + WINDOW_SIZE / 2, k) = complex(i, k).imag();
			}
		}
	}

	return real;
}
