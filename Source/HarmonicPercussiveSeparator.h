/*
  ==============================================================================

    HarmonicPercussiveSeparator.h
    Created: 17 Nov 2015 12:00:42pm
    Author:  Sean

  ==============================================================================
*/

#ifndef HARMONICPERCUSSIVESEPARATOR_H_INCLUDED
#define HARMONICPERCUSSIVESEPARATOR_H_INCLUDED
#include "JuceHeader.h"

class Separator : public Thread {

public:
	Separator();
	~Separator();
	void run();

private:

protected:


};



#endif  // HARMONICPERCUSSIVESEPARATOR_H_INCLUDED
