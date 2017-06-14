/*
  ==============================================================================

    Width.h
    Created: 4 May 2017 4:09:41pm
    Author:  Jimmy

  ==============================================================================
*/

#ifndef WIDTH_H_INCLUDED
#define WIDTH_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
namespace Jimmy {
	namespace DSP {
		class Width {
			float mWidth;
		public:
			Width(): mWidth(0.5f){
			};
			~Width() {
			};

			void setWidth(float width0to1) {
				mWidth = width0to1;
			}
			void process(AudioSampleBuffer& buffer) {
				if (buffer.getNumChannels() > 1) {
					float *inL = buffer.getWritePointer(0);
					float *inR = buffer.getWritePointer(1);
					int numSamples = buffer.getNumSamples();
					for (int i = 0; i < numSamples; i++) {
						processSamples(inL[i], inR[i], inL[i], inR[i]);
					}
				}
			}

			void processSamples(const float inL, const float inR, float &outL, float &outR) {
				float mid = (inL + inR) / sqrt(2.0);
				float side = (inL - inR) / sqrt(2.0);

				mid *= 1.8 * (1 - mWidth);
				side *= 1.8 * mWidth;

				outL = (mid + side) / sqrt(2.0);
				outR = (mid - side) / sqrt(2.0);
			}
		};
	};
};
#endif  // WIDTH_H_INCLUDED
