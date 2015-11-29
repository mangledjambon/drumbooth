/*
  ==============================================================================

    HarmonicPercussiveSeparator.h
    Created: 17 Nov 2015 12:00:42pm
    Author:  Sean

  ==============================================================================
*/

#ifndef HARMONICPERCUSSIVESEPARATOR_H_INCLUDED
#define HARMONICPERCUSSIVESEPARATOR_H_INCLUDED
#include "JuceHeader.h"
#include "Eigen\Eigen"

using Eigen::MatrixXcf;
typedef std::vector<std::complex<float>> ComplexVector;
typedef std::complex<float> Complex;

class Separator : public Thread {

public:

	/*
		Constructor. Takes spectrogram's left and write channel data as input for filtering.
	*/
	Separator(MatrixXcf leftChannelSpec, MatrixXcf rightChannelSpec, int numSamples, int numCols);
	~Separator();

	/*
		Inherited from juce::Thread. Starts the separation process on the given spectrogram.
	*/
	void run();

	/*
		Filters every frame in the spectrogram. Use this method to extract the percussive events.
		This can be resynthesized to audio data by performing an IFFT.

		input:		none
		output:		complex matrix which is spectrogram data for 
					the original spectrogram with the harmonic elements removed
	*/
	MatrixXcf* filterFrames();

	/*
		Filters each frequency bin in the spectrogram. Use this method to extract the harmonic events.
		This can be resynthesized to audio data by performing an IFFT.

		input:		none
		output:		complex matrix containing the frequency data for 
					the original spectrogram with the percussive elements removed.
	*/
	MatrixXcf* filterBins();

	/*
		resynthesizes the spectrograms back to audio data for writing to the file.
	*/
	void resynthesize();

	MatrixXcf filteredSpectro_Perc[2];
	MatrixXcf filteredSpectro_Harm[2];
	MatrixXcf resynth_P[2];
	MatrixXcf resynth_H[2];

private:
	int numColumnsInMatrix;
	bool finished;

	MatrixXcf fullSpectrogramMat_Left, fullSpectrogramMat_Right;
	MatrixXcf complexToMatrix(std::complex<float>* data, int numCols);

	/*
		performs median filter on array of size n. 
	*/
	void median_filter(std::complex<float>* data, int n);

	/* 
		sorts a list of floating-point values of size n.
	*/
	void sort(ComplexVector& vectorToSort);

	int compareElements(Complex c1, Complex c2);

protected:


};



#endif  // HARMONICPERCUSSIVESEPARATOR_H_INCLUDED
