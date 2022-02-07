/*
  ==============================================================================

    GainMaster.h
    Created: 3 May 2017 11:51:41pm
    Author:  Dave Clissold

  ==============================================================================
*/

#ifndef GAINMASTER_H_INCLUDED
#define GAINMASTER_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "Smooth.h"
namespace Life {
	namespace DSP {
		class GainMaster {
			enum {
				GAINDB
				, GAIN0TO1
			};
			int mMode;
			float mGain;

			int mNumChans;
			ScopedPointer<SmoothFilter> mSmooth;
			ScopedPointer<NormalisableRange<float>> mNormalize;
		public:
			GainMaster(int numChans) : mNumChans(numChans) {
				mMode = GAINDB;
				//for (int i = 0; i < mNumChans; i++)
				//	mSmooth.add(SmoothFilter());
				mSmooth = new SmoothFilter();
			};
			GainMaster(float min, float max, int numChans) : mNumChans(numChans) {
				mMode = GAIN0TO1;
				mNormalize = new NormalisableRange<float>(min, max);
				//for (int i = 0; i < numChans; i++)
				//	mSmooth.add(SmoothFilter());
				mSmooth = new SmoothFilter();
			};

			~GainMaster() {
				mNormalize = nullptr;
				mSmooth = nullptr;
			};

			void preparePlay(int sampleRate) {
				mSmooth->preparePlay(0.01, sampleRate);
			}

			void SetGainDB(float gainDb) {
				mGain = dbToGain(gainDb);
				//for (int i = 0; i < mNumChans; i++)
				//	mSmooth.getReference(i).setNewValue(gainDb);
				mSmooth->setNewValue(mGain);
			};

			void SetGainDb0to1(float gainDB0to1) {
				mGain = dbToGain(mNormalize->convertFrom0to1(gainDB0to1));
				mSmooth->setNewValue(mGain);
			};

			void process(AudioSampleBuffer& buffer) {
				float ** dataBuffer = buffer.getArrayOfWritePointers();
				int  numChans = fmin(mNumChans, buffer.getNumChannels());
				int numSamples = buffer.getNumSamples();
				for (int i = 0; i < numSamples; i++) {
					float gain = mSmooth->getValue();
					for (int c = 0; c < numChans; c++) {
						dataBuffer[c][i] = dataBuffer[c][i] * gain;
					}
				}
			}
		private:
			float dbToGain(float db) {
				return Decibels::decibelsToGain<float>(db);
			};

			float smoothGain(float gain) {
			};
		};

	};
};

#endif  // GAINMASTER_H_INCLUDED
