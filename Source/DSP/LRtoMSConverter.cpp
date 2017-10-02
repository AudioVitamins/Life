/*
  ==============================================================================

    LRtoMSConverter.cpp
    Created: 26 Sep 2017 6:03:11pm
    Author:  jrmil

  ==============================================================================
*/

#include "LRtoMSConverter.h"


void LRtoMSConverter::ConvertLRToMid(AudioSampleBuffer &bufferToCopy, AudioSampleBuffer &buffer)
{
	buffer.makeCopyOf(bufferToCopy, true);

	for (int Sample = 0; Sample < buffer.getNumSamples(); Sample++)
	{
		float* OutputL = buffer.getWritePointer(0);
		float* OutputR = buffer.getWritePointer(1);

		const float* InputL = buffer.getReadPointer(0);
		const float* InputR = buffer.getReadPointer(1);

		OutputL[Sample] = InputL[Sample] + InputR[Sample];
		OutputR[Sample] = OutputL[Sample];

	}
}
void LRtoMSConverter::ConvertLRToSide(AudioSampleBuffer &bufferToCopy, AudioSampleBuffer &buffer)
{
	buffer.makeCopyOf(bufferToCopy, true);

	for (int Sample = 0; Sample < buffer.getNumSamples(); Sample++)
	{
		float* OutputL = buffer.getWritePointer(0);
		float* OutputR = buffer.getWritePointer(1);

		const float* InputL = buffer.getReadPointer(0);
		const float* InputR = buffer.getReadPointer(1);

		OutputL[Sample] = InputL[Sample] - InputR[Sample];
		OutputR[Sample] = OutputL[Sample] - 1.0;
	}
	
}
void LRtoMSConverter::ConvertMSToLR(AudioSampleBuffer &Midbuffer, AudioSampleBuffer &Sidebuffer, AudioSampleBuffer &BufferOutput)
{
	for (int Sample = 0; Sample < BufferOutput.getNumSamples(); Sample++)
	{		

		float* OutputL = BufferOutput.getWritePointer(0);
		float* OutputR = BufferOutput.getWritePointer(1);

		const float* Mid = Midbuffer.getReadPointer(0);
		const float* Side = Sidebuffer.getReadPointer(1);

		float InverseSideSample;
		InverseSideSample = Side[Sample];

		if (InverseSideSample <= 0.0) { InverseSideSample = -InverseSideSample; }
		else { InverseSideSample *= -1.0; }

		OutputL[Sample] = (Mid[Sample] + Side[Sample]) * 0.5;
		OutputR[Sample] = (Mid[Sample] + InverseSideSample) * 0.5;
			
	}
	
}