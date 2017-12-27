/*
  ==============================================================================

    Width.h
    Created: 4 May 2017 4:09:41pm
    Author:  Dave Clissold

  ==============================================================================
*/

#ifndef WIDTH_H_INCLUDED
#define WIDTH_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "Smooth.h"
namespace Life {
	namespace DSP {
#if 0
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

				mid *= 2 * (1 - mWidth);
				side *= 2 * mWidth;

				outL = (mid + side) / sqrt(2.0);
				outR = (mid - side) / sqrt(2.0);
			}
		};
#endif
		class Width {
			float mWidth;
			ScopedPointer<SmoothFilter> mSmooth;
		public:
			Width() : mWidth(0.5f) {
				mSmooth = new SmoothFilter();
			};
			~Width() {
				mSmooth = nullptr;
			};

			void preparePlay(float sampleRate) {
				mSmooth->preparePlay(0.01, sampleRate);
			}
			void setWidth(float width0to1) {
				mWidth = width0to1;
				mSmooth->setNewValue(width0to1);
			}
			void process(AudioSampleBuffer& buffer) {
				if (buffer.getNumChannels() > 1) {
					float *inBufferL = buffer.getWritePointer(0);
					float *inBufferR = buffer.getWritePointer(1);
					int numSamples = buffer.getNumSamples();
					for (int i = 0; i < numSamples; i++) {
						const float inL = inBufferL[i];
						const float inR = inBufferR[i];
						float &outL = inBufferL[i];
						float &outR = inBufferR[i];

						float mid = (inL + inR);
						float side = (inL - inR);
						float width = mSmooth->getValue();

						mid *=  (1 - width);
						side *= width;

						outL = (mid + side);
						outR = (mid - side);
					}
				}
			}

		};
	};
};
#endif  // WIDTH_H_INCLUDED
