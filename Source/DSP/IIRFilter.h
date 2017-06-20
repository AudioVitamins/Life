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

			ScopedPointer<LinearSmoothedValue<float>> mSmooth;
		public:
			IIRFilterLP(float sampleRate, int numChans)
				:mSampleRate(sampleRate),
				mNumChans(numChans) {
				mSmooth = new LinearSmoothedValue<float>();
				mSmooth->reset(sampleRate, 0.5);
				for (int i = 0; i < numChans; i++) {
					mIIRFilter.add(juce::IIRFilter());
				}
			};
			~IIRFilterLP() {
				mIIRFilter.clearQuick();
			};
			void changeCutOff(float cutOff) {
				mSmooth->setValue(cutOff);
				//mCoefficients = juce::IIRCoefficients::makeLowPass(mSampleRate, cutOff);
				//for (int i = 0; i < mNumChans; i++) {
				//	mIIRFilter.getReference(i).setCoefficients(mCoefficients);
				//}
			};
			void reset() {
				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).reset();
				}
			}
			void process(AudioBuffer<float> &buffer) {
				float **buffersCh = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();
				for (int i = 0; i < numSamples; i++) {

					float freq = mSmooth->getNextValue();
					mCoefficients = juce::IIRCoefficients::makeLowPass(mSampleRate, freq);
					for (int c = 0; c < mNumChans; c++) {
						mIIRFilter.getReference(c).setCoefficients(mCoefficients);
						buffersCh[c][i] = mIIRFilter.getReference(c).processSingleSampleRaw(buffersCh[c][i]);
					}
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
	};
};



#endif  // IIRFILTER_H_INCLUDED
