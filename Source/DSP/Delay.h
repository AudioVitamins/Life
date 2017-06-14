/*
  ==============================================================================

    Delay.h
    Created: 26 Mar 2017 12:10:10pm
    Author:  Jimmy

  ==============================================================================
*/

#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../LogUtil.h"
namespace Jimmy {
	namespace DSP {
		class StaticDelay {
			float mSampleRate;
			float mMaxMsDelay ;
			float mMinMsDelay;
			float mValueDelaySample;
			float mDelayPoints;
			int mNumChannels;

			//Array<int> mPositionRDelay;
			//Array<int> mPositionWDelay;
			//
			int mWriteIndex;
			int mReadIndex;
			int mNumDelaySamples;
			AudioBuffer<float> mDelayBuffer;

		public:
			StaticDelay(float sampleRate, float minDelay/*In Mili second*/, float maxDelay /*In Mili second*/, int numChannel) :
				mSampleRate(sampleRate),
				mMaxMsDelay(maxDelay),
				mMinMsDelay(minDelay),
				mValueDelaySample(0.0f),
				mNumChannels(numChannel)
			{
				mNumDelaySamples = 0.051 * mSampleRate;
				mDelayBuffer.setSize(mNumChannels, mNumDelaySamples);
				mDelayBuffer.clear();

				mWriteIndex = mReadIndex = 0;
				//mPositionRDelay.resize(mNumChannels);
				//mPositionWDelay.resize(mNumChannels);
			};

			~StaticDelay() {

			};

			void setDelayInMiliSec(float delayInSec) {
				mValueDelaySample = delayInSec;
				mDelayPoints = mSampleRate * mValueDelaySample/1000.0;

				mReadIndex = mWriteIndex - (int)mDelayPoints;
				if (mReadIndex < 0) {
					mReadIndex += mNumDelaySamples;
				}
				/*for (int channel = 0; channel < mNumChannels; ++channel)
				{
					int &posW = mPositionWDelay.getReference(channel);
					int &posR = mPositionRDelay.getReference(channel);
					posR = posW - (int)mDelayPoints;
					if (posR < 0) {
						posR += mNumDelaySamples;
					}
				}*/
			}

			void setDelay(float delay0to1) {
				mValueDelaySample = mMinMsDelay + (mMaxMsDelay - mMinMsDelay) * delay0to1;
				setDelayInMiliSec(mValueDelaySample);
			}

			void preparePlay() {
				
				mDelayBuffer.clear();

				mWriteIndex = 0;

				mDelayPoints = mSampleRate * mValueDelaySample / 1000.0;
				mReadIndex = mWriteIndex - (int)mDelayPoints;
				if (mReadIndex < 0) {
					mReadIndex += mNumDelaySamples;
				}
			}

			void process(AudioBuffer<float> &buffer) {
				int numSamples = buffer.getNumSamples();
				float **delayBuffer = mDelayBuffer.getArrayOfWritePointers();
				float **outputBuffer = buffer.getArrayOfWritePointers();
				for (int i = 0; i < numSamples; i++) {

					int mReadIndex_1 = mReadIndex - 1;
					if (mReadIndex_1 < 0) {
						mReadIndex_1 += mNumDelaySamples;
					}

					for (int ch = 0; ch < mNumChannels; ch++) {
						float xn = delayBuffer[ch][mWriteIndex] = outputBuffer[ch][i];
						float yn = delayBuffer[ch][mReadIndex];
						float yn_1 = delayBuffer[ch][mReadIndex_1];

						float factor = mDelayPoints - (int)mDelayPoints;
						float out = dLinTerp(0.0, 1.0, yn, yn_1, factor);
						outputBuffer[ch][i] = out;
					}
					
					mReadIndex++;
					if (mReadIndex >= mNumDelaySamples)
						mReadIndex -= mNumDelaySamples;
					mWriteIndex++;
					if (mWriteIndex >= mNumDelaySamples)
						mWriteIndex -= mNumDelaySamples;
				}

				//int numSamples = buffer.getNumSamples();
				//// Push To delay
				//for (int channel = 0; channel < mNumChannels; ++channel)
				//{
	
				//	//int &posR = mPositionRDelay.getReference(channel);
				//	//int &posW = mPositionWDelay.getReference(channel);
				//	float *delayWPointer = mDelayBuffer.getWritePointer(channel);
				//	float* outputData = buffer.getWritePointer(channel);
				//	
				//	for (int i = 0; i < numSamples; i++) {
				//		// xn
				//		float xn = outputData[i];
				//		float yn = delayWPointer[posR];

				//		int posR_1 = posR - 1;
				//		if (posR_1 < 0) {
				//			posR_1 += mNumDelaySamples;
				//		}
				//		float yn_1 = delayWPointer[posR_1];
				//		float fFracDelay = mDelayPoints - (int)mDelayPoints;
				//		// Send output
				//		outputData[i] = /*(yn_1) * fFracDelay + (1.0 - fFracDelay) **/ yn;

				//		// Write data to delay buffer
				//		delayWPointer[posW] = xn;
				//		posR++;
				//		if (posR > mNumDelaySamples)
				//			posR -= mNumDelaySamples;
				//		posW++;
				//		if (posW > mNumDelaySamples)
				//			posW -= mNumDelaySamples;
				//	}
				//}
			};

			float dLinTerp(float x1, float x2, float y1, float y2, float x)
			{
				float denom = x2 - x1;
				if (denom == 0)
					return y1; // should not ever happen

							   // calculate decimal position of x
				float dx = (x - x1) / (x2 - x1);

				// use weighted sum method of interpolating
				float result = dx*y2 + (1 - dx)*y1;

				return result;
			}
		};
	};
};
#endif  // DELAY_H_INCLUDED
