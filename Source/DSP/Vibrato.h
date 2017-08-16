/*
  ==============================================================================

	Vibrato.h
	Created: 22 Apr 2017 1:19:05pm
	Author:  Jimmy

  ==============================================================================
*/

#ifndef VIBRATO_H_INCLUDED
#define VIBRATO_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
#include "Lfo.h"
#include "Delay.h"
#include "../LogUtil.h"
#include "Smooth.h"
namespace Jimmy {
	namespace DSP {
		class DelayVibrato {

			/** A basic delay abstraction, with linear interpolation for the reads. */
			class SingleDelay
			{
			public:
				SingleDelay(int bsize)
					:
					buf(1, nextPowerOfTwo(bsize)),
					mask(nextPowerOfTwo(bsize) - 1),
					ptr(mask)
				{
					buf.clear();
					data = buf.getWritePointer(0);
				}

				~SingleDelay()
				{}

				/** Return the value for a fractional delay time in samples. */

				float geti(float delayTime) const
				{
					const auto dInt = int(delayTime);
					const auto dFrac = delayTime - float(dInt);

					const int dlyIndex = (ptr + dInt) & mask;

					const auto a = data[dlyIndex];
					const auto b = data[(dlyIndex + 1) & mask];

					return a * (1.0f - dFrac) + b * dFrac;
				}
				/** Store the sample. Do this before returning delay taps
				for an accurate timing.

				@note ptr points to the sample just written. So when we read later
				with a delay time of zero we actually get a sample!
				*/
				void put(float inSignal)
				{
					data[mask & --ptr] = inSignal;
				}

				float get(int delay) const
				{
					return data[ptr + delay & mask];
				}

				void clear () { buf.clear(); }

			private:
				float* data;
				AudioSampleBuffer buf;
				int mask;
				int ptr;
			};

			class PlayHead
			{
			public:
				/** Returns the current gain of the playhead.  */
				float gain()
				{
					auto g = fade.getNextValue();
					return std::sqrt(g);
				}

				bool stopped() const {
					return !(fade.isSmoothing() || fade.getTargetValue() == 1.0f);
				}

				void setFullGain()
				{
					fade.setValue(1.0f);
					resetSmoother();
				}

				void setZeroGain()
				{
					fade.setValue(0.0f);
					resetSmoother();
				}

				void startFadeIn()
				{
					if (fade.getTargetValue() != 1.0f)
					{
						fade.setValue(0.0f);
						resetSmoother();
						fade.setValue(1.0f);
					}
				}

				void startFadeOut()
				{
					if (fade.getTargetValue() != 0.0f)
					{
						fade.setValue(1.0f);
						resetSmoother();
						fade.setValue(0.0f);
					}
				}

				void resetSmoother()
				{
					const float crossfadeTimeInSeconds = 0.25f;

					fade.reset(sampleRate, crossfadeTimeInSeconds);
				}

				void prepareToPlay(float sampleRate_)
				{
					sampleRate = sampleRate_;
				}

				LinearSmoothedValue<float> fade;

				float delayTime{ 0 };
				float sampleRate;
			};

			PlayHead playheads[2];

			int currentPlayhead{ 0 };
			int mBlockSize{ 1 };

			float mSampleRate;
			int mNumChannels;

			int mNumDelaySamples;

			bool isFirstTimeLoad{ true };
			bool loadNewDelayTime{ false };
			float newTargetDelayInSamples{ 0.0f };

			OwnedArray<SingleDelay> delays;

			// FOr vibrato
			float mFrequency;
			float mDepth;
			LinearSmoothedValue<float> smoothedDepth;
			float mDelayMs;

			int mBufferSize;
			int mDelaySamplesForVibrato;
			float mfDelaySamples;
			Array<int> mReadIdx;
			Array<int> mWriteIdx;

			Array<LFO> mLfo;
			//AudioBuffer<float> mDelayBuffer;
			OwnedArray<SingleDelay> secondDelays;

			Array<SmoothFilter> mSmoothFeedback;
		public:
			DelayVibrato(float sampleRate, float maxDelayMilliseconds, int numChannel) :
				mSampleRate(sampleRate),
				mNumChannels(numChannel),
				// FOr Vibrato
				mFrequency(0.0),
				mDepth(0.0), mDelayMs(0),
				mBufferSize(2 * mSampleRate),
				mDelaySamplesForVibrato(2.0f / 1000.0f * mSampleRate),
				mfDelaySamples(0.0),
				mReadIdx(),
				mWriteIdx(),

				mLfo()
			{
				mNumDelaySamples = maxDelayMilliseconds * mSampleRate;

				for (int i = 0; i < mNumChannels; ++i)
					delays.add(new SingleDelay(mNumDelaySamples));

				playheads[0].prepareToPlay(sampleRate);
				playheads[0].setFullGain();
				playheads[1].prepareToPlay(sampleRate);
				playheads[1].setZeroGain();

				// For vibrato
				for (int c = 0; c < mNumChannels; c++)
				{
					mLfo.add(LFO(mSampleRate));
					mReadIdx.add(0);
					mWriteIdx.add(0);
				}
				//mDelayBuffer.setSize(mNumChannels, mBufferSize);
				//mDelayBuffer.clear();

				for (int i = 0; i < mNumChannels; ++i)
					secondDelays.add(new SingleDelay(mBufferSize));

				for (int c = 0; c < mNumChannels; c++)
					mSmoothFeedback.add(SmoothFilter());
			}

			~DelayVibrato() {}

			void togglePlayhead()
			{
				currentPlayhead = 1 - currentPlayhead;
			}

			PlayHead & getOtherPlayhead()
			{
				return playheads[1 - currentPlayhead];
			}

			void setDelayInMiliSec(float delayInSec)
			{
				if (isFirstTimeLoad)
				{
					playheads[currentPlayhead].delayTime = newTargetDelayInSamples * mSampleRate / 1000.0f;
					isFirstTimeLoad = false;
				}
				else
				{
					loadNewDelayTime = true;
					newTargetDelayInSamples = delayInSec;
				}
			}
			//For vibrato
			void SetFrequency(float frequency) {
				for (int c = 0; c < mNumChannels; c++) {
					mLfo.getRawDataPointer()[c].SetFrequency(frequency);
				}
			}

			// Amount
			void SetDepth(float depth) {
				mDepth = depth / 10;
				smoothedDepth.setValue(mDepth);
			}

			void SetFeedback(float feedBackPct) {
				for (int c = 0; c < mNumChannels; c++) {
					mSmoothFeedback.getReference(c).setNewValue(feedBackPct);
				}
			};

			void preparePlay(float sampleRate, int blockSize)
			{
				mBlockSize = blockSize;

				// For Vibrato
				//mDelayBuffer.clear();
				for (auto d : secondDelays)
					d->clear();

				for (int c = 0; c < mNumChannels; c++) {
					mLfo.getReference(c).preparePlay();
					mWriteIdx.getReference(c) = 0;
					mReadIdx.getReference(c) = 0;
				}

				for (int c = 0; c < mNumChannels; c++) 
					mSmoothFeedback.getReference(c).preparePlay(0.01, sampleRate);

				smoothedDepth.reset(sampleRate, 0.1);
			}

			void updateDelayTime()
			{
				if (loadNewDelayTime && getOtherPlayhead().stopped())
				{
					playheads[currentPlayhead].startFadeOut();
					togglePlayhead();
					playheads[currentPlayhead].startFadeIn();
					playheads[currentPlayhead].delayTime = newTargetDelayInSamples * mSampleRate / 1000.0f;
					loadNewDelayTime = false;
				}
			}

			void process(AudioBuffer<float> &buffer)
			{
				updateDelayTime();

				auto numSamples = buffer.getNumSamples();
				auto numChannels = buffer.getNumChannels();

				auto audio = buffer.getArrayOfWritePointers();
				//auto delayBuffer = mDelayBuffer.getArrayOfWritePointers();

				for (int i = 0; i < numSamples; ++i)
				{
					float headGains[2];

					headGains[0] = playheads[0].gain();
					headGains[1] = playheads[1].gain();

					for (int chan = 0; chan < numChannels; ++chan)
					{
						float summedDelayOutputs{ 0.0f };

						for (int headIndex = 0; headIndex < 2; ++headIndex)
						{
							if (!playheads[headIndex].stopped())
								summedDelayOutputs += delays[chan]->geti(playheads[headIndex].delayTime) * headGains[headIndex];
						}

						LFO &lfo = mLfo.getRawDataPointer()[chan];
						SmoothFilter &smoothedFeedback = mSmoothFeedback.getReference(chan);

						float delay = floor(smoothedDepth.getNextValue() * mDelaySamplesForVibrato);

						float offset = 1.0f + delay + lfo.Value() * delay;

						auto secondDelayOutput = secondDelays[chan]->geti(offset);

						float originalInput = audio[chan][i];

						audio[chan][i] = secondDelayOutput;

						auto feedback = smoothedFeedback.getValue() / 100.0f;

						secondDelays[chan]->put(summedDelayOutputs);

						float input	= (1.0f - feedback) * originalInput + feedback * secondDelayOutput;

						delays[chan]->put(input);
					}
				}
			};
		};

		class Vibrato {
            float mSampleRate;
            int mNumChans;
			float mFrequency;
			float mDepth;
			float mFeedback;
			float mDelayMs;
			int mBufferSize;
            Array<LFO> mLfo;
			Array<int> mReadIdx;
            Array<int> mWriteIdx;
            float mfDelaySamples;
            
            int mDelaySamplesForVibrato;
            
			AudioBuffer<float> mDelayBuffer;

			Array<SmoothFilter> mSmoothFeedback;
		public:
			Vibrato(float sampleRate, int nChans) :
				mSampleRate(sampleRate),
				mNumChans(nChans),
				mFrequency(0.0),
				mDepth(0.0), mFeedback(0), mDelayMs(0),
				mBufferSize(2 * mSampleRate),
				mLfo(),
				mReadIdx(),
				mWriteIdx(),
				//mDelaySamples(0),
				mfDelaySamples(0.0),
				mDelaySamplesForVibrato(2.0f / 1000.0f * mSampleRate),
				mDelayBuffer()
			{
				for (int c = 0; c < mNumChans; c++)
				{
					mLfo.add(LFO(mSampleRate));
					mReadIdx.add(0);
					mWriteIdx.add(0);
				}
				mDelayBuffer.setSize(nChans, mBufferSize);
				mDelayBuffer.clear();
				for (int c = 0; c < mNumChans; c++)
				{
					mSmoothFeedback.add(SmoothFilter());
				}
			}

			~Vibrato() {

			}

		

			void preparePlay(float sampleRate) {
				mDelayBuffer.clear();
				for (int c = 0; c < mNumChans; c++) {
					mLfo.getReference(c).preparePlay();
					mWriteIdx.getReference(c) = 0;
					mReadIdx.getReference(c) = 0;
				}
				for (int c = 0; c < mNumChans; c++) {
					mSmoothFeedback.getReference(c).preparePlay(0.01, sampleRate);
				}
			}
			void SetFrequency(float frequency) {
				for (int c = 0; c < mNumChans; c++) {
					mLfo.getRawDataPointer()[c].SetFrequency(frequency);
				}
			}
			// Amount
			void SetDepth(float depth) {
				mDepth = depth / 10;
			};

			void SetFeedback(float feedBackPct) {
				//mFeedback = feedBackPct;
				for (int c = 0; c < mNumChans; c++) {
					mSmoothFeedback.getReference(c).setNewValue(feedBackPct);
				}
			};

			void process(AudioBuffer<float> &buffer) {
				int numSamples = buffer.getNumSamples();

				for (int c = 0; c < mNumChans; c++) {
					const float *inputBuffer = buffer.getReadPointer(c);
					float *outputBuffer = buffer.getWritePointer(c);
					float *delayBuffer = mDelayBuffer.getWritePointer(c);

					LFO &lfo = mLfo.getRawDataPointer()[c];
					int &writeIdx = mWriteIdx.getRawDataPointer()[c];
					
					SmoothFilter &smooth = mSmoothFeedback.getReference(c);
					for (int i = 0; i < numSamples; i++) {

 						float modFreq = lfo.Value();
						int delay = floor( mDepth * mDelaySamplesForVibrato);
						float offset = 1 + delay +  modFreq * delay;
						int readIdx = (writeIdx - (int)floor(offset) + mBufferSize) % mBufferSize;
						int nReadIndex_1 = (readIdx - 1 + mBufferSize) % mBufferSize;
						float frac = offset - floor(offset);

						float xn = inputBuffer[i];
						float yn = delayBuffer[readIdx];
						float yn_1 = delayBuffer[nReadIndex_1];
						float fInterp = yn_1 * frac + (1 - frac)* yn;
						outputBuffer[i] = fInterp;
						mFeedback = smooth.getValue();
						delayBuffer[writeIdx] = (1.0 - mFeedback / 100.0) * xn + mFeedback /100.0 * fInterp;
						// Incr Write index
						writeIdx = (writeIdx + 1) % mBufferSize;
					}
				}
			}

		private:
			float dLinTerp(float x1, float x2, float y1, float y2, float x)
			{
				float denom = x2 - x1;
				if (denom == 0)
					return y1; // should not ever happen

							   // calculate decimal position of x
				float dx = (x - x1) / (x2 - x1);

				// use weighted sum method of interpolating
				float result = dx*y2 + (1 - dx)*y1;

				return result;
			}
			float getSampleHermite4p3o(float x, float y1, float y2, float y3, float y4)
			{
				float c0, c1, c2, c3;

				// 4-point, 3rd-order Hermite (x-form)
				c0 = y2;
				c1 = (1.0 / 2.0)*(y3 - y1);
				c2 = (y1 - (5.0 / 2.0)*y2) + (2.0*y3 - (1.0 / 2.0)*y4);
				c3 = (1.0 / 2.0)*(y4 - y1) + (3.0 / 2.0)*(y2 - y3);
				return ((c3*x + c2)*x + c1)*x + c0;
			}
		};
	}
}



#endif  // VIBRATO_H_INCLUDED
