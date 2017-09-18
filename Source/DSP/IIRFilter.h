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

			//class PlayHead
			//{
			//public:
			//	/** Returns the current gain of the playhead.  */
			//	float gain()
			//	{
			//		auto g = fade.getNextValue();
			//		return std::sqrt(g);
			//	}
			//	bool stopped() const {
			//		return !(fade.isSmoothing() || fade.getTargetValue() == 1.0f);
			//	}
			//	void setFullGain()
			//	{
			//		fade.setValue(1.0f);
			//		resetSmoother();
			//	}
			//	void setZeroGain()
			//	{
			//		fade.setValue(0.0f);
			//		resetSmoother();
			//	}
			//	void startFadeIn()
			//	{
			//		if (fade.getTargetValue() != 1.0f)
			//		{
			//			fade.setValue(0.0f);
			//			resetSmoother();
			//			fade.setValue(1.0f);
			//		}
			//	}
			//	void startFadeOut()
			//	{
			//		if (fade.getTargetValue() != 0.0f)
			//		{
			//			fade.setValue(1.0f);
			//			resetSmoother();
			//			fade.setValue(0.0f);
			//		}
			//	}
			//	void resetSmoother()
			//	{
			//		const float crossfadeTimeInSeconds = 0.25f;
			//		fade.reset(sampleRate, crossfadeTimeInSeconds);
			//	}
			//	void prepareToPlay(float sampleRate_)
			//	{
			//		sampleRate = sampleRate_;
			//	}
			//	LinearSmoothedValue<float> fade;
			//	float delayTime{ 0 };
			//	float sampleRate;
			//};
			//PlayHead playheads[2];
			//int currentPlayhead{ 0 };
			//int mBlockSize{ 1 };
			//bool isFirstTimeLoad{ true };
			//bool loadNewDelayTime{ false };
		public:
			IIRFilterLP(float sampleRate, int numChans)
				:mSampleRate(sampleRate),
				mNumChans(numChans) {
				mSmooth = new LinearSmoothedValue<float>();
				mSmooth->reset(sampleRate, 0.1);
				for (int i = 0; i < numChans; i++) {
					mIIRFilter.add(juce::IIRFilter());
				}

				/*playheads[0].prepareToPlay(sampleRate);
				playheads[0].setFullGain();
				playheads[1].prepareToPlay(sampleRate);
				playheads[1].setZeroGain();*/
			};

			~IIRFilterLP() {
				mIIRFilter.clearQuick();
			};
			void changeCutOff(float cutOff) {
				mSmooth->setValue(cutOff);
				/*mCoefficients = juce::IIRCoefficients::makeLowPass(mSampleRate, cutOff);
				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).setCoefficients(mCoefficients);
				}*/
			};
			/*void togglePlayhead()
			{
				currentPlayhead = 1 - currentPlayhead;
			}

			PlayHead & getOtherPlayhead()
			{
				return playheads[1 - currentPlayhead];
			}
			void updateDelayTime()
			{
				if (getOtherPlayhead().stopped())
				{
					playheads[currentPlayhead].startFadeOut();
					togglePlayhead();
					playheads[currentPlayhead].startFadeIn();
				}
			}*/
			void process(AudioBuffer<float> &buffer, int Chan) {
				float **buffersCh = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();
				for (int i = 0; i < numSamples; i++) {

					float freq = mSmooth->getNextValue();
					mCoefficients = juce::IIRCoefficients::makeLowPass(mSampleRate, freq);
					
						mIIRFilter.getReference(Chan).setCoefficients(mCoefficients);
						buffersCh[Chan][i] = mIIRFilter.getReference(Chan).processSingleSampleRaw(buffersCh[Chan][i]);
					
				}
			};
			void reset() {
				/*for (int i = 0; i < mNumChans; i++) {
				mIIRFilter.getReference(i).reset();
				}*/
			}
		};
		class IIRFilterHP {
			Array<juce::IIRFilter> mIIRFilter;
			float mSampleRate;
			int mNumChans;
			juce::IIRCoefficients mCoefficients;

			ScopedPointer<LinearSmoothedValue<float>> mSmooth;
		public:
			IIRFilterHP(float sampleRate, int numChans)
				:mSampleRate(sampleRate),
				mNumChans(numChans) {
				for (int i = 0; i < numChans; i++) {
					mIIRFilter.add(juce::IIRFilter());
				}
				mSmooth = new LinearSmoothedValue<float>();
				mSmooth->reset(sampleRate, 0.1);
			};
			~IIRFilterHP() {
				mIIRFilter.clearQuick();
			};
			void changeCutOff(float cutOff) {
				mSmooth->setValue(cutOff);
				//mCoefficients = juce::IIRCoefficients::makeHighPass(mSampleRate, cutOff);
				//for (int i = 0; i < mNumChans; i++) {
				//	mIIRFilter.getReference(i).setCoefficients(mCoefficients);
				//}
			};
			void process(AudioBuffer<float> &buffer, int Chan) {
				/*float **buffersCh = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();
				for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).processSamples(buffersCh[i], numSamples);
				}*/

				float **buffersCh = buffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();
				for (int i = 0; i < numSamples; i++) {

					float freq = mSmooth->getNextValue();
					mCoefficients = juce::IIRCoefficients::makeHighPass(mSampleRate, freq);
					
						mIIRFilter.getReference(Chan).setCoefficients(mCoefficients);
						buffersCh[Chan][i] = mIIRFilter.getReference(Chan).processSingleSampleRaw(buffersCh[Chan][i]);
					
				}

			};
			void reset() {
				/*for (int i = 0; i < mNumChans; i++) {
					mIIRFilter.getReference(i).reset();
				}*/
			}
		};
	};
};



#endif  // IIRFILTER_H_INCLUDED
