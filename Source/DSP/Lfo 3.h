/*
  ==============================================================================

    Lfo.h
    Created: 19 Apr 2017 12:36:33am
    Author:  Dave Clissold

  ==============================================================================
*/

#ifndef LFO_H_INCLUDED
#define LFO_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace Life {
	namespace DSP {
		class LFO {
			int mSampleRate;
			float mFrequency;
			float mCurrentPhase;
		public:
			LFO(int sampleRate) : mSampleRate(sampleRate),
				 mFrequency(0.0f),
				 mCurrentPhase(0.0f){
			};

			~LFO() {
			};

			void SetFrequency(float freq) {
				mFrequency = freq;
			}

			void SetPhase(float Phase) {
				mCurrentPhase = Phase;
			}

			void preparePlay() {
				mCurrentPhase = 0.0;
			}
			
			float Value() {
				const float dp = 2.0f * float_Pi * mFrequency / mSampleRate; // phase step

				float value = sin(mCurrentPhase);
				value = (value /*+ 1*/) /* 0.5f*/; // transform from [-1; 1] to [0; 1]

				mCurrentPhase += dp;
				if (mCurrentPhase > 2.0f * float_Pi)
					mCurrentPhase -= 2.0f * float_Pi;

				return value;
			}
		};
	};
};



#endif  // LFO_H_INCLUDED
