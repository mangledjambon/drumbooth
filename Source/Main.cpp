/*
  ==============================================================================

    This file contains code to read an audio file from the disk and perform
	median filtering on the audio data.

	TODO: 
			fix mp3 support (currently just reads 0s)
			median filter implementation
			write file to disk

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include "transforms\STFT.h"
#include "transforms\ISTFT.h"
#include "Constants.h"
#include "Eigen\Eigen"
#include "HarmonicPercussiveSeparator.h"

using std::cout;

//==============================================================================
int main (int argc, char* argv[])
{

	const int NUM_INPUT_FILES = argc - 1;
	File* inputFiles = new File[NUM_INPUT_FILES];
	String fileName;
	String fileNameNoExt;
	String fileExtension;
	STFT stft;
	ISTFT istft;
	
	// initialise format manager which handles reading different file formats
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
	
	if (argc < 2)
	{
		// no parameters entered
		cout << "No file selected." << newLine;
		return 1;
	}

	for (int i = 0; i < NUM_INPUT_FILES; i++) 
	{
		inputFiles[i] = argv[i + 1];

		// check if file exists
		if (!inputFiles[i].exists())
		{
			cout << "File not found.";
			return 1;
		}

		fileName = inputFiles[i].getFileName();
		fileNameNoExt = inputFiles[i].getFileNameWithoutExtension();
		fileExtension = inputFiles[i].getFileExtension();
		cout << "Added file " << fileName;

		// READ FILE ======================
		ScopedPointer<AudioFormatReader> formatReader = formatManager.createReaderFor(inputFiles[i]);
		int numChannels = formatReader->numChannels;
		int64 numSamples = formatReader->lengthInSamples;

		cout << " (" << numChannels << " channels, " << numSamples << " samples.)";

		AudioSampleBuffer samples(numChannels, numSamples);
		samples.clear();

		int numCols = 1 + floor((numSamples - WINDOW_SIZE) / HOP_SIZE);
		int startSample = 0;
		int readerStartSample = 0;

		// read audio file into AudioSampleBuffer samples
		formatReader->read(&samples, startSample, numSamples, readerStartSample, true, true);
		// ================================

		/*
		// DEBUG: DISPLAY SAMPLE VALUES ===
		// loop through each channel
		for (int channel = 0; channel < numChannels; channel++)
		{
			// get non-writeable pointer to buffer data
			//bufferData[channel] = (float*)samples.getReadPointer(channel);

			// this will print the first 50 samples from a channel of audio
			cout << "\nChannel " << channel << ": ";
			for (int sample = 0; sample < 50; sample++) 
			{
				cout << samples.getSample(channel, sample) << ", ";
			}
			cout << "\n==============================";
		}
		// ================================
		*/

		/*
		
			! 
				Perform FFT -> Original Spectrogram (frequency domain)
			->	Median filter -> Percussion and Harmonic spectrograms
			->	perform Inverse FFT on Per. & Harm. spectrograms to transform back to audio (time domain)
			->	write audio data to two files (one containing harmonic, one containing percussion)
			!
		
		*/

		// Transform to Spectrogram Data
		float* bufferData[2];
		std::complex<float>* spectrogramData_Left, * spectrogramData_Right;
		stft.initWindow(1);

		bufferData[0] = (float*)samples.getReadPointer(0, 0);
		stft.applyWindowFunction(bufferData[0]);
		spectrogramData_Left = stft.realToComplex(stft.performForwardTransform(bufferData[0]));

		bufferData[1] = (float*)samples.getReadPointer(1, 0);
		stft.applyWindowFunction(bufferData[1]);
		spectrogramData_Right = stft.realToComplex(stft.performForwardTransform(bufferData[1]));
		// ================================

		// send spectrogram data to separator here and return filtered spectrograms
		Separator separator;
		separator.startThread();
		bool finished = separator.waitForThreadToExit(500);
		// =======================================

		// Transform back to audio data -- NEEDS WORK!
		Array<float> outputSignal_Left, outputSignal_Right; 
		float * ifftResults_Left, * ifftResults_Right;
		Eigen::MatrixXf realMatrix_Left, realMatrix_Right;
		float temp_L[4096], temp_R[4096];

		istft.initWindow(1);
		realMatrix_Left = MatrixXf::Zero((WINDOW_SIZE / 2) + 1, numCols);
		realMatrix_Right = MatrixXf::Zero((WINDOW_SIZE / 2) + 1, numCols);
		ifftResults_Left[0] = 0;
		ifftResults_Right[0] = 0;

		// add-overlap ====================
		int offset = 0;
		for (int col = 0; col < numCols; col++)
		{
			for (int row = 0; row < WINDOW_SIZE / 2; row++)
			{
				temp_L[row] = realMatrix_Left(row, col);
				temp_R[row] = realMatrix_Right(row, col);
			}

			istft.performInverseTransform(temp_L, ifftResults_Left);
			istft.rescale(ifftResults_Left);
			istft.performInverseTransform(temp_R, ifftResults_Right);
			istft.rescale(ifftResults_Right);

			for (int i = 0; i < WINDOW_SIZE; i++)
			{
				outputSignal_Left.set(offset + i, (outputSignal_Left[offset + i] + (ifftResults_Left[i] * istft.window[i])));
				outputSignal_Right.set(offset + i, (outputSignal_Right[offset + i] + (ifftResults_Right[i] * istft.window[i])));
			}

			offset += HOP_SIZE;
		}
		// ================================

		// WRITE FILE =====================
		FileOutputStream* output;
		File outputFile = File(File::getCurrentWorkingDirectory().getChildFile(fileNameNoExt + "_perc" + fileExtension));

		if (outputFile.exists())
		{
			outputFile.deleteFile();
		}

		output = outputFile.createOutputStream();
		WavAudioFormat* wavFormat = new WavAudioFormat();
		AudioFormatWriter* writer = wavFormat->createWriterFor(output, 44100.0, numChannels, 16, NULL, 0);
		writer->writeFromAudioSampleBuffer(samples, 0, numSamples);
		writer->flush();
		delete writer;
		delete wavFormat;
		wavFormat = nullptr;
		writer = nullptr;

		cout << "\nFile written.";
		// ================================

		cout << newLine;
		system("pause");
		
		}	
	
    return 0;
}
