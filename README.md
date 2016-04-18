# DrumBooth Prototype v0.2
# DCOM4 Final Year Project 2015
Sean Breen - R00070693
sean.breen@mycit.ie

DrumBooth takes an audio file and separates it into its harmonic and percussive components.
Currently only tested on Windows. Linux version soon.
Built with JUCE (www.juce.com), FFTReal (http://ldesoras.free.fr/prod.html) and Eigen (http://eigen.tuxfamily.org/).

Usage:

	> MedianFiltering.exe [file]

where file is an audio file in WAV, FLAC or MP3 format with sample rate of 44.1kHz. 

![screenshot1](https://github.com/mangledjambon/drumbooth/blob/master/screenshots/screenshot.JPG "CLI")

Original:
![screenshot2](https://github.com/mangledjambon/drumbooth/blob/master/screenshots/original.JPG "Wave1")

Extracted Harmonic Components:
![screenshot3](https://github.com/mangledjambon/drumbooth/blob/master/screenshots/extracted_harmonic.JPG "Wave2")

Extracted Percussive Components:
![screenshot4](https://github.com/mangledjambon/drumbooth/blob/master/screenshots/extracted_percussive.JPG "wave3")

Best result are achieved with high-quality wav or flac files, less than 3 minutes and 30 seconds long. Any file greater than 3 minutes and 30 seconds in length may cause the application to crash. A fix for this is incoming. 
