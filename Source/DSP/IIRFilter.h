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
			ScopedPointer<juce::IIRFilter> mIIRFilter;
			float mSampleRate;
			int mNumChans;
			juce::IIRCoefficients mCoefficients;
		public:
			IIRFilterLP(float sampleRate, int numChans)
				:mSampleRate(sampleRate),
				mNumChans(numChans) {
				mIIRFilter = new juce::IIRFilter();
			};
			~IIRFilterLP() {
				mIIRFilter = nullptr;
			};
			void changeCutOff(float cutOff) {
				mCoefficients = juce::IIRCoefficients::makeLowPass(mSampleRate, cutOff);
				mIIRFilter->setCoefficients(mCoefficients);
			};

			void process(AudioBuffer<float> &buffer) {
				float **buffersCh = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();

				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter->processSamples(buffersCh[i], numSamples);
				}
				
			};
			float process(float in) {
				return mIIRFilter->processSingleSampleRaw(in);
			}
		};
		class IIRFilterHP {
			ScopedPointer<juce::IIRFilter> mIIRFilter;
			float mSampleRate;
			int mNumChans;
			juce::IIRCoefficients mCoefficients;
		public:
			IIRFilterHP(float sampleRate, int numChans)
				:mSampleRate(sampleRate),
				mNumChans(numChans) {
				mIIRFilter = new juce::IIRFilter();
			};
			~IIRFilterHP() {
				mIIRFilter = nullptr;
			};
			void changeCutOff(float cutOff) {
				mCoefficients = juce::IIRCoefficients::makeHighPass(mSampleRate, cutOff);
				mIIRFilter->setCoefficients(mCoefficients);
			};
			void process(AudioBuffer<float> &buffer) {
				float **buffersCh = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();

				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter->processSamples(buffersCh[i], numSamples);
				}

			};
			float process(float in) {
				return mIIRFilter->processSingleSampleRaw(in);
			}
		};

		class IIRFilterBP {
			ScopedPointer<juce::IIRFilter> mIIRFilter;
			float mSampleRate;
			int mNumChans;
			juce::IIRCoefficients mCoefficients;
		public:
			IIRFilterBP(float sampleRate, int numChans)
				:mSampleRate(sampleRate),
				mNumChans(numChans) {
				mIIRFilter = new juce::IIRFilter();
			};
			~IIRFilterBP() {
				mIIRFilter = nullptr;
			};
			void changeCutOff(float cutOff) {
				mCoefficients = juce::IIRCoefficients::makeLowPass(mSampleRate, cutOff);
				mIIRFilter->setCoefficients(mCoefficients);
			};

			void process(AudioBuffer<float> &buffer) {
				float **buffersCh = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();

				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter->processSamples(buffersCh[i], numSamples);
				}

			};
			float process(float in) {
				return mIIRFilter->processSingleSampleRaw(in);
			}
		};
	};
};



#endif  // IIRFILTER_H_INCLUDED
