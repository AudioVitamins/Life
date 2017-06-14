/*
  ==============================================================================

    WetDry.h
    Created: 5 May 2017 2:57:40am
    Author:  Jimmy

  ==============================================================================
*/

#ifndef WETDRY_H_INCLUDED
#define WETDRY_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "Smooth.h"
namespace Jimmy {
	namespace DSP {
		class WetDry {
			float mWet;
			ScopedPointer<SmoothFilter> mSmooth;
		public:
			WetDry() : mWet(0.5f) {
				mSmooth = new SmoothFilter();
			}; 
			~WetDry() {
			};

			void preparePlay(int sampleRate) {
				mSmooth->preparePlay(0.01, sampleRate);
			}

			void setWet(float wet0to1) {
				mSmooth->setNewValue(wet0to1);
			}

			void process(AudioSampleBuffer& rawBuffer, AudioSampleBuffer& outBuffer) {
				int numSamples = outBuffer.getNumSamples();
				int numChans = outBuffer.getNumChannels();
				const float **raw = rawBuffer.getArrayOfReadPointers();
				float **out = outBuffer.getArrayOfWritePointers();
				for (int i = 0; i < numSamples; i++) {
					mWet = mSmooth->getValue();
					for (int c = 0; c < numChans; c++) {
						out[c][i] = processSamples(raw[c][i], out[c][i]);
					}
				}
			}

			float processSamples(const float xn, const float yn) {
				return mWet*yn + (1.0 - mWet)*xn;
			}
		};
	};
};
#endif  // WETDRY_H_INCLUDED
