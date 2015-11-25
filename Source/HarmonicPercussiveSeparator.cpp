/*
  ==============================================================================

    HarmonicPercussiveSeparator.cpp
    Created: 17 Nov 2015 12:00:42pm
    Author:  Sean

  ==============================================================================
*/

#include <iostream>
#include "HarmonicPercussiveSeparator.h"

Separator::Separator(std::complex<float>* leftChannelData, std::complex<float>* rightChannelData, int numSamples, int numCols) : Thread("Separator")
{
	numColumnsInMatrix = numCols;
	fullSpectrogramMat_Left = MatrixXcf::Zero(2049, numCols);
	fullSpectrogramMat_Right = MatrixXcf::Zero(2049, numCols);

	fullSpectrogramMat_Left = complexToMatrix(leftChannelData, numCols);
	fullSpectrogramMat_Right = complexToMatrix(rightChannelData, numCols);
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
	//MatrixXcf matrix = Eigen::MatrixXcf::Zero(2049, cols);
	Eigen::Map<MatrixXcf> matrix(data, 2049, cols); // this method will work, just figure out how to map strides for 2049 x numColumns at runtime
	return matrix;
}
