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

namespace Jimmy {
	namespace DSP {
		class StaticDelay {
			float mSampleRate;
			float mMaxMsDelay ;
			float mMinMsDelay;
			float mValueDelaySample;
			int mDelayPoints;
			int mNumChannels;

			Array<int> mPositionRDelay;
			Array<int> mPositionWDelay;
			//
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
				mNumDelaySamples = 6 * mSampleRate;
				mDelayBuffer.setSize(mNumChannels, mNumDelaySamples);
				mDelayBuffer.clear();

				mPositionRDelay.resize(mNumChannels);
				mPositionWDelay.resize(mNumChannels);
			};

			~StaticDelay() {

			};

			void setDelayInMiliSec(float delayInSec) {
				mValueDelaySample = delayInSec;
				mDelayPoints = mSampleRate * mValueDelaySample/1000.0;
				
			}

			void setDelay(float delay0to1) {
				mValueDelaySample = mMinMsDelay + (mMaxMsDelay - mMinMsDelay) * delay0to1;
				setDelayInMiliSec(mValueDelaySample);
			}

			void process(AudioBuffer<float> &buffer) {
				int numSamples = buffer.getNumSamples();
				// Push To delay
				for (int channel = 0; channel < mNumChannels; ++channel)
				{
	
					int &posR = mPositionRDelay.getReference(channel);
					int &posW = mPositionWDelay.getReference(channel);
					int posRDelay = posR - mDelayPoints;
					

					float *delayWPointer = mDelayBuffer.getWritePointer(channel);
					float* outputData = buffer.getWritePointer(channel);

					for (int i = 0; i < numSamples; i++) {
						if (posR == mNumDelaySamples) posR = 0;
						if (posRDelay >= mNumDelaySamples) posRDelay = 0;
						if (posW == mNumDelaySamples) posW = 0;

						delayWPointer[posW] = outputData[i];
						if (posRDelay >= 0) {
							outputData[i] = delayWPointer[posRDelay];
						}
						else {
							outputData[i] = 0.0;
						}

						posRDelay++;
						posW++;
						posR++;
					}
				}
			};

		};
	};
};
#endif  // DELAY_H_INCLUDED
