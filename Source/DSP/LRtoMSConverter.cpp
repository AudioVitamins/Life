/*
  ==============================================================================

    LRtoMSConverter.cpp
    Created: 26 Sep 2017 6:03:11pm
    Author:  jrmil

  ==============================================================================
*/

#include "LRtoMSConverter.h"


void LRtoMSConverter::ConvertLRToMid(AudioSampleBuffer &buffer)
{
	// Converts LR to mid, Output on left channel only
	for (int Sample = 0; Sample < buffer.getNumSamples(); Sample++)
	{
		float* OutputL = buffer.getWritePointer(0);

		const float* InputL = buffer.getReadPointer(0);
		const float* InputR = buffer.getReadPointer(1);

		OutputL[Sample] = InputL[Sample] + InputR[Sample];
	}
}
void LRtoMSConverter::ConvertLRToSide(AudioSampleBuffer &buffer, AudioSampleBuffer &SideBuffer)
{
	// Converts LR to side, Output on right channel only.
	// Uses &SideBuffer as an original copy of the buffer because 
	// &buffer may already be converted to Mid.
	for (int Sample = 0; Sample < buffer.getNumSamples(); Sample++)
	{
		float* OutputR = buffer.getWritePointer(1);

		float* SideOutL = SideBuffer.getWritePointer(0);
		float* SideOutR = SideBuffer.getWritePointer(1);

		OutputR[Sample] = SideOutL[Sample] - SideOutR[Sample];
	}
}
void LRtoMSConverter::ConvertMSToLR(AudioSampleBuffer &buffer)
{
	// Decodes Mid (Left) and Side (Right) back to LR
	for (int Sample = 0; Sample < buffer.getNumSamples(); Sample++)
	{		
		float* OutputL = buffer.getWritePointer(0);
		float* OutputR = buffer.getWritePointer(1);

		const float* Mid = buffer.getReadPointer(0);
		const float* Side = buffer.getReadPointer(1);

		OutputL[Sample] = (Mid[Sample] + Side[Sample]) * 0.5;
		OutputR[Sample] = (Mid[Sample] - Side[Sample]);
	}	
}