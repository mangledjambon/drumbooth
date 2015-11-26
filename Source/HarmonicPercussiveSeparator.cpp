/*
  ==============================================================================

    HarmonicPercussiveSeparator.cpp
    Created: 17 Nov 2015 12:00:42pm
    Author:  Sean

	TODO:	filterBins	->	loop through frequency bins, 
							for each bin do:			
								1. take 17 values
								2. sort
								3. take median value

			filterFrames -> loop through frames
							for each frame do:
								take 17 samples
								sort them
								take median value
								add frame to end of filter spectrogram
  ==============================================================================
*/

#include <iostream>
#include "HarmonicPercussiveSeparator.h"
#define FILTER_SIZE 17

using std::cout;

Separator::Separator(MatrixXcf leftChannelData, MatrixXcf rightChannelData, int numSamples, int numCols) : Thread("Separator"), finished(false)
{
	fullSpectrogramMat_Left = MatrixXcf::Zero(2049, numCols);
	fullSpectrogramMat_Right = MatrixXcf::Zero(2049, numCols);
	fullSpectrogramMat_Left = leftChannelData;
	fullSpectrogramMat_Right = rightChannelData;

	cout<< newLine
		<< getThreadName()
		<< ": Created new separator with rows = "
		<< fullSpectrogramMat_Left.rows()
		<< ", cols = "
		<< fullSpectrogramMat_Left.cols()
		<< newLine;
}

Separator::~Separator()
{
	cout << "Thread destructor called.";
}

void Separator::run()
{
		// this should check periodically if the threadShouldExit() == true
	if (threadShouldExit())
		return;

	//filterFrames(); // makes thread fail when exiting (??)

	if (threadShouldExit())
		return;

	filterBins();

	if (threadShouldExit())
		return;

	resynthesize();

}

MatrixXcf* Separator::filterFrames() 
{
	//std::vector<float>* frame[2];

	cout << "\nFiltering Frames..." << newLine;

	for (int col = 0; col < fullSpectrogramMat_Left.cols(); col++)
	{
		for (int row = 0; row < fullSpectrogramMat_Left.rows(); col++)
		{

		}
	}

	return *filteredSpectro_Perc;
}

MatrixXcf* Separator::filterBins()
{
	cout << "\nFiltering Frequency Bins..." << newLine;

	for (int row = 0; row < fullSpectrogramMat_Left.rows(); row++)
	{			
		for (int col = 0; col < fullSpectrogramMat_Left.cols(); col++)
		{

		}
	}

	return *filteredSpectro_Harm;
}

void Separator::resynthesize()
{
	/*

	Pest = filteredSpectrogram_Perc
	Hest = filteredSpectrogram_Harm

	Masks for audio:

		P = OriginalSpectrogram x Pest^2 / (Pest^2 + Hest^2 + eps)

		H = OriginalSpectrogram x Hest^2 / (Pest^2 + Hest^2 + eps)

	do istft on P & H.

	*/

	cout << newLine << "Resynthesizing..." << newLine;

	// resynth perc
	

	// resynth harm
}

MatrixXcf Separator::complexToMatrix(std::complex<float>* data, int numColumns) 
{
	const int cols = (const int)numColumns;
	int offset = 0;
	MatrixXcf matrix = Eigen::MatrixXcf::Zero(2049, cols);
	
	for (int col = 0; col < numColumns; col++)
	{
		for (int row = 0; row < 2049; row++)
		{
			matrix(row, col) = data[row + offset];
		}

		offset += 2049;
	}

	return matrix;
}
