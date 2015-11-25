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

Separator::Separator(MatrixXcf leftChannelData, MatrixXcf rightChannelData, int numSamples, int numCols) : Thread("Separator")
{
	fullSpectrogramMat_Left = MatrixXcf::Zero(2049, numCols);
	fullSpectrogramMat_Right = MatrixXcf::Zero(2049, numCols);
	fullSpectrogramMat_Left = leftChannelData;
	fullSpectrogramMat_Right = rightChannelData;
}

Separator::~Separator()
{}

void Separator::run()
{
	// perform thread's code here!
	//printf("%o"+ getThreadName());
	printf("\nRows: %d\nCols: %d\n", fullSpectrogramMat_Left.rows(), fullSpectrogramMat_Left.cols());
	printf("Filtering...");
	filterBins();
	filterFrames();
}

MatrixXcf* Separator::filterFrames() 
{
	filteredSpectro_Perc[0] = &fullSpectrogramMat_Left;
	filteredSpectro_Perc[1] = &fullSpectrogramMat_Right;
	return *filteredSpectro_Perc;
}

MatrixXcf* Separator::filterBins()
{
	filteredSpectro_Harm[0] = new MatrixXcf(fullSpectrogramMat_Left);
	filteredSpectro_Harm[1] = new MatrixXcf(fullSpectrogramMat_Right);
	return *filteredSpectro_Harm;
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
