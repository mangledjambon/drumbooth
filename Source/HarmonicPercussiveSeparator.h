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

class Separator : public Thread {

public:

	/*
		Constructor. Takes a spectrogram as input for filtering.
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

	MatrixXcf* filteredSpectro_Perc[2];
	MatrixXcf* filteredSpectro_Harm[2];
private:
	int numColumnsInMatrix;

	MatrixXcf fullSpectrogramMat_Left, fullSpectrogramMat_Right;
	MatrixXcf complexToMatrix(std::complex<float>* data, int numCols);
protected:


};



#endif  // HARMONICPERCUSSIVESEPARATOR_H_INCLUDED
