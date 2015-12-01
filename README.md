# DrumBooth
# DCOM4 Final Year Project 2015
Sean Breen - R00070693
sean.breen@mycit.ie

DrumBooth takes an audio file and separates it into its harmonic and percussive components.
Currently only tested on Windows. Linux version soon.
Built with JUCE, FFTReal and Eigen.

Usage:

	> MedianFiltering.exe [file]

where file is an audio file in .wav or .mp3 format.

Best result are achieved with high-quality wav files, less than 3 minutes and 30 seconds long. Any file greater than 3 minutes and 30 seconds may cause the application to crash. A fix for this is incoming. 