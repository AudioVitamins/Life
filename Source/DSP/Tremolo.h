/*
  ==============================================================================

    Tremolo.h
    Created: 26 Apr 2017 11:25:13pm
    Author:  Jimmy

  ==============================================================================
*/

#ifndef TREMOLO_H_INCLUDED
#define TREMOLO_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "Lfo.h"
#include "Delay.h"

namespace Jimmy {
	namespace DSP {
		class Tremolo {
			float mSampleRate;
			float mFrequency;
			float mDepth;

			int mNumChans;

			LFO mLfo;
		public:
			Tremolo(float sampleRate, int nChans) :
				mSampleRate(sampleRate),
				mNumChans(nChans),
				mLfo(sampleRate) {

			};

			~Tremolo() {

			};

			void SetFrequency(float frequency) {
				mLfo.SetFrequency(frequency);
			};
			void SetPhase(float phase) {
				mLfo.SetPhase(phase);
			}
			void SetDepth(float depth) {
				mDepth = depth;
			};

			void process(AudioBuffer<float> &buffer, int Chan) {
				const float **input = buffer.getArrayOfReadPointers();
				float **output = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();
				for (int i = 0; i < numSamples; i++) {
					float lfo = mLfo.Value();
					// first multiply the value by depth/100
					float gain = mDepth / 30.0 * (lfo - 1)  + 1;
						output[Chan][i] = input[Chan][i] * gain;
					
				}
			}
		};
	};
};

#endif  // TREMOLO_H_INCLUDED
