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
	void ConvertLRToMid(AudioSampleBuffer &bufferToCopy, AudioSampleBuffer &buffer);
	void ConvertLRToSide(AudioSampleBuffer &buffertoCopy, AudioSampleBuffer &buffer);
	void ConvertMSToLR(AudioSampleBuffer &Midbuffer, AudioSampleBuffer &Sidebuffer, AudioSampleBuffer &BufferOutput);
	
};

#endif  // LRTOMSCONVERTER_H_INCLUDED
