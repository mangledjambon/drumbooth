/*
  ==============================================================================

    HarmonicPercussiveSeparator.cpp
    Created: 17 Nov 2015 12:00:42pm
    Author:  Sean

  ==============================================================================
*/

#include <iostream>
#include "HarmonicPercussiveSeparator.h"

Separator::Separator() : Thread("Separator")
{}

Separator::~Separator()
{}

void Separator::run()
{
	// perform thread's code here!
	std::cout << "\nRunning separation algorithm..." << newLine;
}
