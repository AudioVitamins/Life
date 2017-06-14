/*
  ==============================================================================

    IIRFilter.h
    Created: 4 May 2017 1:48:38am
    Author:  Jimmy

  ==============================================================================
*/

#ifndef IIRFILTER_H_INCLUDED
#define IIRFILTER_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
namespace Jimmy {
	namespace DSP {
		class IIRFilterLP {
			Array<juce::IIRFilter> mIIRFilter;
			float mSampleRate;
			int mNumChans;
			juce::IIRCoefficients mCoefficients;
		public:
			IIRFilterLP(float sampleRate, int numChans)
				:mSampleRate(sampleRate),
				mNumChans(numChans) {
				for (int i = 0; i < numChans; i++) {
					mIIRFilter.add(juce::IIRFilter());
				}
			};
			~IIRFilterLP() {
				mIIRFilter.clearQuick();
			};
			void changeCutOff(float cutOff) {
				mCoefficients = juce::IIRCoefficients::makeLowPass(mSampleRate, cutOff);
				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).setCoefficients(mCoefficients);
				}
			};
			void reset() {
				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).reset();
				}
			}
			void process(AudioBuffer<float> &buffer) {
				float **buffersCh = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();

				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).processSamples(buffersCh[i], numSamples);
				}
				
			};
			void setSampleRate(float sample) {
				mSampleRate = sample;
			}
		};
		class IIRFilterHP {
			Array<juce::IIRFilter> mIIRFilter;
			float mSampleRate;
			int mNumChans;
			juce::IIRCoefficients mCoefficients;
		public:
			IIRFilterHP(float sampleRate, int numChans)
				:mSampleRate(sampleRate),
				mNumChans(numChans) {
				for (int i = 0; i < numChans; i++) {
					mIIRFilter.add(juce::IIRFilter());
				}
			};
			~IIRFilterHP() {
				mIIRFilter.clearQuick();
			};
			void changeCutOff(float cutOff) {
				mCoefficients = juce::IIRCoefficients::makeHighPass(mSampleRate, cutOff);
				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).setCoefficients(mCoefficients);
				}
			};
			void process(AudioBuffer<float> &buffer) {
				float **buffersCh = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();

				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).processSamples(buffersCh[i], numSamples);
				}

			};
			void setSampleRate(float sample) {
				mSampleRate = sample;
			}
			void reset() {
				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).reset();
				}
			}
		};
		class IIRFilterBP {
			Array<juce::IIRFilter> mIIRFilter;
			float mSampleRate;
			int mNumChans;
			juce::IIRCoefficients mCoefficients;
		public:
			IIRFilterBP(float sampleRate, int numChans)
				:mSampleRate(sampleRate),
				mNumChans(numChans) {
				for (int i = 0; i < numChans; i++) {
					mIIRFilter.add(juce::IIRFilter());
				}
			};
			~IIRFilterBP() {
				mIIRFilter.clearQuick();
			};
			void changeCutOff(float cutOff) {
				mCoefficients = juce::IIRCoefficients::makeLowPass(mSampleRate, cutOff);
				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).setCoefficients(mCoefficients);
				}
			};
			void reset() {
				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).reset();
				}
			}
			void setSampleRate(float sample) {
				mSampleRate = sample;
			}
			void process(AudioBuffer<float> &buffer) {
				float **buffersCh = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();

				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).processSamples(buffersCh[i], numSamples);
				}
			};
		};
	};
};



#endif  // IIRFILTER_H_INCLUDED
