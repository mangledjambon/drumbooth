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
#include <vector>
#include "HarmonicPercussiveSeparator.h"
#define FILTER_SIZE 17
#define MEDIAN_INDEX (FILTER_SIZE / 2) + 1
#define eps 0.000001f

using std::cout;
typedef std::complex<float> Complex;
typedef std::vector<std::complex<float>> ComplexVector;
class MyArraySorter;

class MyArraySorter
{
public:

	static int compareElements(float a, float b)
	{
		if (a < b)
			return -1;
		else if (a > b)
			return 1;
		else // if a == b
			return 0;
	}
};

Separator::Separator(MatrixXcf leftChannelData, MatrixXcf rightChannelData, int numSamples, int numCols) : Thread("Separator"), finished(false)
{
	// complex spectrogram data for left and right channels
	fullSpectrogramMat_Left = MatrixXcf::Zero(2049, numCols);
	fullSpectrogramMat_Right = MatrixXcf::Zero(2049, numCols);

	// real data for filtered spectrograms
	filteredSpectro_Harm[0] = MatrixXf::Zero(2049, numCols);
	filteredSpectro_Harm[1] = MatrixXf::Zero(2049, numCols);
	filteredSpectro_Perc[0] = MatrixXf::Zero(2049, numCols);
	filteredSpectro_Perc[1] = MatrixXf::Zero(2049, numCols);

	// assign channel data to complex matrices
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

MatrixXf* Separator::filterFrames() 
{

	cout << "\nFiltering Frames..." << newLine;
	int numCols = fullSpectrogramMat_Left.cols();
	cout << "\r0%" << std::flush;

	// loop columns
	for (int col = 0; col < numCols; col++)
	{
		// loop rows
		for (int row = 0; row < fullSpectrogramMat_Left.rows(); row++)
		{
			Array<float> frameData[2];
			MyArraySorter arraySorter;
			float median;

			for (int samp = row; samp < (row + FILTER_SIZE); samp++)
			{
				if (samp < 2049)
				{
					frameData[0].add(std::abs(fullSpectrogramMat_Left(samp, col)));
					frameData[1].add(std::abs(fullSpectrogramMat_Right(samp, col)));
				}

				if (frameData[0].size() == FILTER_SIZE) // when all samples in filter batch
				{
					// sort arrays
					frameData[0].sort(arraySorter);
					frameData[1].sort(arraySorter);

					// assign median value to filteredSpectro_Perc[0](row, col)
					median = frameData[0][MEDIAN_INDEX];
					filteredSpectro_Perc[0](row, col) = median;

					// assign median value to filteredSpectro_Perc[1](row, col)
					median = frameData[1][MEDIAN_INDEX];
					filteredSpectro_Perc[1](row, col) = median;
				}
			}
		}

		if (col == numCols / 4)
			cout << "\r25%" << std::flush;
		else if (col == numCols / 2)
			cout << "\r50%" << std::flush;
		else if (col == numCols / 3)
			cout << "\r66%" << std::flush;
		else if (col == numCols - 1)
			cout << "\r100%" << std::flush << newLine;
		
	}

	return filteredSpectro_Perc;
}

MatrixXf* Separator::filterBins()
{
	cout << "\nFiltering Frequency Bins..." << newLine;
	int numRows = fullSpectrogramMat_Left.rows();
	cout << "\r0%" << std::flush;

	// loop through rows of spectrogram
	for (int row = 0; row < numRows; row++)
	{	
		// loop through columns of spectrogram
		for (int col = 0; col < fullSpectrogramMat_Left.cols(); col++)
		{
			Array<float> binData[2];
			MyArraySorter arraySorter;
			float median;

			for (int samp = col; samp < (col + FILTER_SIZE); samp++)
			{
				if (samp < fullSpectrogramMat_Left.cols())
				{
					binData[0].add(std::abs(fullSpectrogramMat_Left(row, samp)));
					binData[1].add(std::abs(fullSpectrogramMat_Right(row, samp)));
				}

				if (binData[0].size() == FILTER_SIZE) // when all samples in filter batch
				{
					// sort filter batch
					binData[0].sort(arraySorter);
					binData[1].sort(arraySorter);

					// assign median value to filteredSpectro_Harm[0](row, col)
					median = binData[0][MEDIAN_INDEX];
					filteredSpectro_Harm[0](row, col) = median;

					// assign median value to filteredSpectro_Harm[1](row, col)
					median = binData[1][MEDIAN_INDEX];
					filteredSpectro_Harm[1](row, col) = median;

				}
			}
		}

		if (row == numRows / 10)
			cout << "\r10%" << std::flush;
		else if (row == numRows / 8)
			cout << "\r20%" << std::flush;
		else if (row == numRows / 2)
			cout << "\r50%" << std::flush;
		else if (row == numRows / 0.5)
			cout << "\r75%" << std::flush;
		else if (row == numRows - 1)
			cout << "\r100%" << std::flush << newLine;

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

		where
			Pest = spectrogram for filtered frame data
			Hest = spectrogram for filtered frequency bin data
			eps = constant

	do istft on P & H in main.

	*/

	cout << newLine << "Resynthesizing..." << newLine;
	cout << "\r0%" << std::flush;
	
	MatrixXf pest2[2];
	pest2[0] = filteredSpectro_Perc[0].cwiseProduct(filteredSpectro_Perc[0]);
	pest2[1] = filteredSpectro_Perc[1].cwiseProduct(filteredSpectro_Perc[1]);
	cout << "\r15%" << std::flush;

	MatrixXf hest2[2];
	hest2[0] = filteredSpectro_Harm[0].cwiseProduct(filteredSpectro_Harm[0]);
	hest2[1] = filteredSpectro_Harm[1].cwiseProduct(filteredSpectro_Harm[1]);
	cout << "\r30%" << std::flush;

	MatrixXf maskDivisorL = (pest2[0] + hest2[0]);
	MatrixXf maskDivisorR = (pest2[1] + hest2[1]);
	cout << "\r50%" << std::flush;

	MatrixXf p_maskL = pest2[0].cwiseQuotient(maskDivisorL);
	MatrixXf p_maskR = pest2[1].cwiseQuotient(maskDivisorR);
	cout << "\r75%" << std::flush;

	MatrixXf h_maskL = hest2[0].cwiseQuotient(maskDivisorL);
	MatrixXf h_maskR = hest2[1].cwiseQuotient(maskDivisorR);
	cout << "\r90%" << std::flush;

	resynth_P[0] = fullSpectrogramMat_Left.cwiseProduct(p_maskL);
	resynth_P[1] = fullSpectrogramMat_Right.cwiseProduct(p_maskR);
	cout << "\r95%" << std::flush;

	resynth_H[0] = fullSpectrogramMat_Left.cwiseProduct(h_maskL);
	resynth_H[1] = fullSpectrogramMat_Right.cwiseProduct(h_maskR);
	cout << "\r100%" << std::flush << newLine;
}
