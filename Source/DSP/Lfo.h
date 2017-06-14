/*
  ==============================================================================

    Lfo.h
    Created: 19 Apr 2017 12:36:33am
    Author:  Jimmy

  ==============================================================================
*/

#ifndef LFO_H_INCLUDED
#define LFO_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace Jimmy {
	namespace DSP {
		class LFO {
			int mSampleRate;
			float mPhase;
			float mFrequency;
			float mCurrentPhase;
		public:
			LFO(int sampleRate) : mSampleRate(sampleRate),
				mPhase(0.0f), mFrequency(0.0f){
			};

			~LFO() {
			};

			void SetFrequency(float freq) {
				mFrequency = freq;
			}
			void SetPhase(float phase) {
				mCurrentPhase -= mPhase;
				mPhase = phase;
				mCurrentPhase += mPhase;
			}
			float Value() {
				const float dp = 2 * float_Pi * mFrequency / mSampleRate; // phase step

				float value = sin(mPhase);
				value = (value + 1) * 0.5; // transform from [-1; 1] to [0; 1]

				mPhase += dp;
				while (mPhase > 2 * float_Pi)
					mPhase -= 2 * float_Pi;

				return value;
			}
		};
	};
};



#endif  // LFO_H_INCLUDED
