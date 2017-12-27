/*
  ==============================================================================

    LRtoMSConverter.h
    Created: 26 Sep 2017 6:03:11pm
    Author:  jrmil

  ==============================================================================
*/

#ifndef LRTOMSCONVERTER_H_INCLUDED
#define LRTOMSCONVERTER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class LRtoMSConverter
{
private:
public:
	void ConvertLRToMid( AudioSampleBuffer &buffer);
	void ConvertLRToSide(AudioSampleBuffer &buffer, AudioSampleBuffer &SideBuffer);
	void ConvertMSToLR(AudioSampleBuffer &buffer);
};

#endif  // LRTOMSCONVERTER_H_INCLUDED
