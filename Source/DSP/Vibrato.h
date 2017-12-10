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
<<<<<<< HEAD
                    // floating point in accuracies mean getNextValue() can
                    // sometimes overshoot the target value, giving a negative
                    // number. So we ensure that the value passed to std::sqrt
                    // is always greater than or equal to 0.0f.
                    auto g = fade.getNextValue();
                    auto r = std::sqrt(jmax(0.0f, g));
                    return r;

=======
					// floating point in accuracies mean getNextValue() can
					// sometimes overshoot the target value, giving a negative
					// number. So we ensure that the value passed to std::sqrt 
					// is always greater than or equal to 0.0f.
					auto g = fade.getNextValue(); 
					auto r = std::sqrt(jmax(0.0f, g));
					return r;
>>>>>>> d20b8b6 (fixed fatal bug where some sample rates wouldn't play back@)
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
			LinearSmoothedValue<float> smoothedVibratoDepth;
			float mDelayMs;

			int mBufferSize;
			float mDelaySamplesForVibrato;
			float mfDelaySamples;
			Array<int> mReadIdx;
			Array<int> mWriteIdx;

			Array<LFO> mLfo;
			//AudioBuffer<float> mDelayBuffer;
			OwnedArray<SingleDelay> vibratoDelays;

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
					vibratoDelays.add(new SingleDelay(mBufferSize));

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
				smoothedVibratoDepth.setValue(mDepth);
			}

			void SetPhase(float Phase) {
				for (int c = 0; c < mNumChannels; c++) {
					mLfo.getRawDataPointer()[c].SetPhase(Phase);
				}
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
				for (auto d : vibratoDelays)
					d->clear();

				for (int c = 0; c < mNumChannels; c++) {
					mLfo.getReference(c).preparePlay();
					mWriteIdx.getReference(c) = 0;
					mReadIdx.getReference(c) = 0;
				}

				for (int c = 0; c < mNumChannels; c++) 
					mSmoothFeedback.getReference(c).preparePlay(0.01, sampleRate);

				smoothedVibratoDepth.reset(sampleRate, 0.3);
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

			void process(AudioBuffer<float> &buffer, int chan)
			{
				updateDelayTime();

				auto numSamples = buffer.getNumSamples();
			//	auto numChannels = buffer.getNumChannels();

				auto audio = buffer.getArrayOfWritePointers();
				//auto delayBuffer = mDelayBuffer.getArrayOfWritePointers();

				for (int i = 0; i < numSamples; ++i)
				{
					float headGains[2];

					headGains[0] = playheads[0].gain();
					headGains[1] = playheads[1].gain();

//					for (int chan = 0; chan < numChannels; ++chan)
		//			{
						float summedDelayOutputs{ 0.0f };

						for (int headIndex = 0; headIndex < 2; ++headIndex)
						{
							if (!playheads[headIndex].stopped())
								summedDelayOutputs += delays[chan]->geti(playheads[headIndex].delayTime) * headGains[headIndex];
<<<<<<< HEAD
                            
                            jassert(!isnan(summedDelayOutputs));

=======

							jassert(!isnan(summedDelayOutputs));
>>>>>>> d20b8b6 (fixed fatal bug where some sample rates wouldn't play back@)
						}

						LFO &lfo = mLfo.getRawDataPointer()[chan];
						SmoothFilter &smoothedFeedback = mSmoothFeedback.getReference(chan);

						float vibratoDelayLength = smoothedVibratoDepth.getNextValue() * mDelaySamplesForVibrato;

<<<<<<< HEAD
                        float vibratoLfoAppliedDelayLength = 1.0f + vibratoDelayLength + lfo.Value() * vibratoDelayLength;
                        //float vibratoLfoAppliedDelayLength = 1.0f;

=======
						//float vibratoLfoAppliedDelayLength = 1.0f + vibratoDelayLength + lfo.Value() * vibratoDelayLength;
						float vibratoLfoAppliedDelayLength = 1.0f;
>>>>>>> d20b8b6 (fixed fatal bug where some sample rates wouldn't play back@)

						auto vibratoOut = vibratoDelays[chan]->geti(vibratoLfoAppliedDelayLength);

						float originalInput = audio[chan][i];

						audio[chan][i] = vibratoOut;

						auto feedback = smoothedFeedback.getValue() / 100.0f;

						vibratoDelays[chan]->put(summedDelayOutputs);

						float input	= (1.0f - feedback) * originalInput + feedback * vibratoOut;

						delays[chan]->put(input);
				//	}
				}
			}
		};
	}
}



#endif  // VIBRATO_H_INCLUDED
