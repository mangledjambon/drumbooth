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
#include <algorithm>
#include <vector>
#include "HarmonicPercussiveSeparator.h"
#define FILTER_SIZE 17
#define eps 0.000001f

using std::cout;
typedef std::complex<float> Complex;
typedef std::vector<std::complex<float>> ComplexVector;

Separator::Separator(MatrixXcf leftChannelData, MatrixXcf rightChannelData, int numSamples, int numCols) : Thread("Separator"), finished(false)
{
	fullSpectrogramMat_Left = MatrixXcf::Zero(2049, numCols);
	fullSpectrogramMat_Right = MatrixXcf::Zero(2049, numCols);
	filteredSpectro_Harm[0] = MatrixXcf::Zero(2049, numCols);
	filteredSpectro_Harm[1] = MatrixXcf::Zero(2049, numCols);
	filteredSpectro_Perc[0] = MatrixXcf::Zero(2049, numCols);
	filteredSpectro_Perc[1] = MatrixXcf::Zero(2049, numCols);
	fullSpectrogramMat_Left << leftChannelData;
	fullSpectrogramMat_Right << rightChannelData;

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

	filterFrames();
	filterBins();
	resynthesize();

}

MatrixXcf* Separator::filterFrames() 
{

	cout << "\nFiltering Frames..." << newLine;

	// loop columns
	for (int col = 0; col < fullSpectrogramMat_Left.cols(); col++)
	{
		// loop rows
		for (int row = 0; row < fullSpectrogramMat_Left.rows(); row++)
		{
			// store frameData in Array
			//frameData[0].set(row, fullSpectrogramMat_Left(row, col));
			//cout << frameData[0][row] << newLine;
			//frameData[1].set(row, fullSpectrogramMat_Left(row, col));
			//cout << frameData[1][row] << newLine;

			Array<Complex> frameData[2];
			Complex median;
			for (int samp = row; samp < (row + FILTER_SIZE); samp++)
			{
				if (samp < 2049)
				{
					frameData[0].add(fullSpectrogramMat_Left(samp, col));
					frameData[1].add(fullSpectrogramMat_Right(samp, col));
				}

				if (frameData[0].size() == FILTER_SIZE) // 17 samples in filter batch
				{
					// sort and pick median
					for (int sortSamp = 0; sortSamp <= (FILTER_SIZE - 1); sortSamp++)
					{
						// fix sorting!!
						if (frameData[0][sortSamp].real() > frameData[0][sortSamp + 1].real())
							frameData[0].swap(sortSamp, sortSamp+1);

						if (frameData[1][sortSamp].real() > frameData[1][sortSamp + 1].real())
							frameData[1].swap(sortSamp, sortSamp + 1);
					}

					// assign median value to filteredSpectro_Perc[0](row, col)
					median = frameData[0][9];
					filteredSpectro_Perc[0](row, col) = median;

					// assign median value to filteredSpectro_Perc[1](row, col)
					median = frameData[1][9];
					filteredSpectro_Perc[1](row, col) = median;

				}
			}
		}
		
	}

	return filteredSpectro_Perc;
}

MatrixXcf* Separator::filterBins()
{
	cout << "\nFiltering Frequency Bins..." << newLine;

	// loop through rows of spectrogram
	for (int row = 0; row < fullSpectrogramMat_Left.rows(); row++)
	{	
		// fill vectors with sepctrogram data for this row
		for (int col = 0; col < fullSpectrogramMat_Left.cols(); col++)
		{
			Array<Complex> binData[2];
			Complex median;
			for (int samp = col; samp < (col + FILTER_SIZE); samp++)
			{
				if (samp < fullSpectrogramMat_Left.cols())
				{
					binData[0].add(fullSpectrogramMat_Left(row, samp));
					binData[1].add(fullSpectrogramMat_Right(row, samp));
				}

				if (binData[0].size() == FILTER_SIZE) // 17 samples in filter batch
				{
					// sort and pick median
					for (int sortSamp = 0; sortSamp <= (FILTER_SIZE - 1); sortSamp++)
					{
						// fix sorting!!
						if (binData[0][sortSamp].real() > binData[0][sortSamp + 1].real())
							binData[0].swap(sortSamp, sortSamp + 1);

						if (binData[1][sortSamp].real() > binData[1][sortSamp + 1].real())
							binData[1].swap(sortSamp, sortSamp + 1);
					}

					// assign median value to filteredSpectro_Perc[0](row, col)
					median = binData[0][9];
					filteredSpectro_Harm[0](row, col) = median;

					// assign median value to filteredSpectro_Perc[1](row, col)
					median = binData[1][9];
					filteredSpectro_Harm[1](row, col) = median;

				}
			}
		}
	}

	return filteredSpectro_Harm;
}

void Separator::resynthesize()
{
	/*

	Pest = filteredSpectrogram_Perc
	Hest = filteredSpectrogram_Harm

	Masks for audio:

		P = OriginalSpectrogram x Pest^2 / (Pest^2 + Hest^2 + eps)

		H = OriginalSpectrogram x Hest^2 / (Pest^2 + Hest^2 + eps)

	do istft on P & H in main.

	*/

	cout << newLine << "Resynthesizing..." << newLine;
	
	MatrixXcf pest2[2];
	pest2[0] = filteredSpectro_Perc[0].cwiseProduct(filteredSpectro_Perc[0]);
	pest2[1] = filteredSpectro_Perc[1].cwiseProduct(filteredSpectro_Perc[1]);

	MatrixXcf pest = pest2[0] + pest2[1];

	MatrixXcf hest2[2];
	hest2[0] = filteredSpectro_Harm[0].cwiseProduct(filteredSpectro_Harm[0]);
	hest2[1] = filteredSpectro_Harm[1].cwiseProduct(filteredSpectro_Harm[1]);

	MatrixXcf hest = hest2[0] + hest2[1];

	MatrixXcf maskDivisor = (pest + hest);

	MatrixXcf p_mask = pest.cwiseQuotient(maskDivisor);

	MatrixXcf h_mask = hest.cwiseQuotient(maskDivisor);

	filteredSpectro_Perc[0] = fullSpectrogramMat_Left.cwiseProduct(p_mask);
	filteredSpectro_Perc[1] = fullSpectrogramMat_Right.cwiseProduct(p_mask);

	filteredSpectro_Harm[0] = fullSpectrogramMat_Left.cwiseProduct(h_mask);
	filteredSpectro_Harm[1] = fullSpectrogramMat_Right.cwiseProduct(h_mask);
}

// must test this comparator
int Separator::compareElements(Complex c1,Complex c2)
{
	if (c1.real() == c2.real())
		return 0;
	if (c1.real() < c2.real())
		return 1;
	if (c1.real() < c2.real())
		return -1;

	return 2;
}

void Separator::sort(ComplexVector& dataToSort)
{
	//std::sort(dataToSort.begin(), dataToSort.end(), myWay);
	Array<std::complex<float>> comp;
	//comp.s

	for (int i = 0; i < dataToSort.size() - 1; i++)
	{
		Complex temp;

		if (dataToSort[i].real() > dataToSort[i + 1].real())
		{
			temp = dataToSort[i];
			dataToSort[i] = dataToSort[i + 1];
			dataToSort[i + 1] = temp;
		}
	}
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
