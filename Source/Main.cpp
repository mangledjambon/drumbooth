/*
  ==============================================================================

    This file contains code to read an audio file from the disk and perform
	median filtering on the audio data.

	TODO: 
			fix mp3 support (currently just reads 0s)	
			Filter spectrogram data.

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

		// read whole audio file into AudioSampleBuffer samples
		cout << newLine << "\nReading file into buffer.";
		formatReader->read(&samples, startSample, numSamples, readerStartSample, true, true);
		cout << "\nBuffer filled.";
		// ================================


		/* Convert to spectrogram (frequency domain) data using FFT. */

		// print length of buffer
		cout << "\nBuffer length is " << samples.getNumSamples();
		cout << "\nInitialising FFT with window size " << WINDOW_SIZE;
		ScopedPointer<STFT> stft = new STFT(WINDOW_SIZE);
		stft->initWindow(1);

		float* bufferData[2];
		bufferData[0] = new float[4096];
		bufferData[1] = new float[4096];

		MatrixXcf spectrogram_L = MatrixXcf::Zero(2049, numCols);
		MatrixXcf spectrogram_R = MatrixXcf::Zero(2049, numCols);
		std::complex<float>* spectrogramData_L = new std::complex<float>[(WINDOW_SIZE/2) + 1];
		std::complex<float>* spectrogramData_R = new std::complex<float>[(WINDOW_SIZE/2) + 1];

		cout << "\n\nConverting to spectrogram...\n";
		startSample = 0;
		for (int col = 0; col < numCols; col++)
		{
			bufferData[0] = (float*)samples.getReadPointer(0, startSample);
			bufferData[1] = (float*)samples.getReadPointer(1, startSample);

			//stft->applyWindowFunction(bufferData[0]);
			//stft->applyWindowFunction(bufferData[1]);

			float* bL = stft->performForwardTransform(bufferData[0]);
			float* bR = stft->performForwardTransform(bufferData[1]);


			spectrogramData_L = stft->realToComplex(bL, WINDOW_SIZE);
			spectrogramData_R =  stft->realToComplex(bR, WINDOW_SIZE);

			//cout << *spectrogramData_L << " ";
			//cout << *spectrogramData_R << newLine;

			for (int sample = 0; sample < 2049; sample++)
			{
				spectrogram_L(sample, col) = spectrogramData_L[sample];
				spectrogram_R(sample, col) = spectrogramData_R[sample];
			}

			startSample += HOP_SIZE;
		}

		/*
		// DEBUG: DISPLAY SAMPLE VALUES ===
		// loop through each channel
		for (int channel = 0; channel < numChannels; channel++)
		{
			// get non-writeable pointer to buffer data
			//bufferData[channel] = (float*)samples.getReadPointer(channel);

			// this will print the first 50 samples from a channel of audio
			cout << "\nChannel " << channel << ": ";
			for (int sample = 0; sample < 2049; sample++)
			{
			//cout << samples.getSample(channel, sample) << ", ";
			//cout << bufferData[channel][sample] << ", ";
			if (channel == 0) { cout << spectrogram_L(sample, 0); }
			if (channel == 1) { cout << spectrogram_R(sample, 0); }
			}
			cout << "\n==============================";
		}
		*/
		// ================================
		
	
		// ================================

		// send spectrogram data to separator here and return filtered spectrograms
		//ScopedPointer<Separator> separator = new Separator(spectrogram_L, spectrogram_R, numSamples, numCols);
		//separator->startThread();
		//bool finished = separator->waitForThreadToExit(5000);
		// =======================================

		
		Array<float> outputSignal_Left, outputSignal_Right; 
		float ifftResults_Left[WINDOW_SIZE], ifftResults_Right[WINDOW_SIZE];
		float temp_L[WINDOW_SIZE] = {};
		float temp_R[WINDOW_SIZE] = {};

		Eigen::MatrixXf realMatrix_Left, realMatrix_Right;
		realMatrix_Left = MatrixXf::Zero(WINDOW_SIZE, numCols);
		realMatrix_Right = MatrixXf::Zero(WINDOW_SIZE, numCols);

		istft.initWindow(1);

		realMatrix_Left = istft.complexToReal(spectrogram_L);
		realMatrix_Right = istft.complexToReal(spectrogram_R);
		
		for (int i = 0; i < numSamples; i++)
		{
			outputSignal_Left.set(i, 0.0f);
			outputSignal_Right.set(i, 0.0f);
		}

		// add-overlap ====================
		int offset = 0;
		for (int col = 0; col < numCols; col++)
		{
			for (int row = 0; row < WINDOW_SIZE; row++)
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

		//outputSignal_Left.minimiseStorageOverheads();
		//outputSignal_Right.minimiseStorageOverheads();
		// ================================

		// WRITE FILE =====================
		float gain = 0.5f;
		juce::AudioSampleBuffer outSamples(2, numSamples);
		outSamples.clear();
		const float* leftData = outputSignal_Left.getRawDataPointer();
		const float* rightData = outputSignal_Right.getRawDataPointer();

		outSamples.addFrom(0, 0, leftData, numSamples, gain);
		outSamples.addFrom(1, 0, rightData, numSamples, gain);

		FileOutputStream* output;
		File* outputFile = new File(File::getCurrentWorkingDirectory().getChildFile(fileNameNoExt + "_test" + fileExtension));

		if (outputFile->exists())
		{
			outputFile->deleteFile();
		}

		output = outputFile->createOutputStream();
		WavAudioFormat* wavFormat = new WavAudioFormat();
		AudioFormatWriter* writer = wavFormat->createWriterFor(output, 44100.0, numChannels, 16, NULL, 0);
		writer->flush();
		writer->writeFromAudioSampleBuffer(outSamples, 0, numSamples);
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
