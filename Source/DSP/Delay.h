/*
  ==============================================================================

    Delay.h
    Created: 26 Mar 2017 12:10:10pm
    Author:  Jimmy

  ==============================================================================
*/

#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../LogUtil.h"
#include "Smooth.h"
namespace Jimmy {
	namespace DSP {
		class StaticDelay {

            /** A basic delay abstraction, with linear interpolation for the reads. */
            class SingleDelay
            {
            public:
                SingleDelay (int bsize)
                    :
                    buf (1, nextPowerOfTwo (bsize)),
                    mask (nextPowerOfTwo (bsize) - 1),
                    ptr (mask)
                {
                    buf.clear();
                    data = buf.getWritePointer (0);
                }

                ~SingleDelay()
                {}

                /** Return the value for a fractional delay time in samples. */

                float geti (float delayTime) const
                {
                    const auto dInt = int (delayTime);
                    const auto dFrac = delayTime - float (dInt);

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
                void put (float inSignal)
                {
                    data[mask & --ptr] = inSignal;
                }

                float get (int delay) const
                {
                    return data[ptr + delay & mask];
                }

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

                bool stopped() const { return !(fade.isSmoothing() || fade.getTargetValue() == 1.0f); }

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

		public:
			StaticDelay(float sampleRate, float maxDelayMilliseconds, int numChannel) :
				mSampleRate(sampleRate),
				mNumChannels(numChannel)
			{
				mNumDelaySamples = maxDelayMilliseconds * mSampleRate;

                for (int i = 0; i < numChannel; ++i)
                    delays.add(new SingleDelay(mNumDelaySamples));

                playheads[0].prepareToPlay(sampleRate);
                playheads[0].setFullGain();
                playheads[1].prepareToPlay(sampleRate);
                playheads[1].setZeroGain();
			}

			~StaticDelay() {}

            void togglePlayhead ()
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

			void preparePlay(int blockSize) 
		    {
                mBlockSize = blockSize;
			}

            void updateDelayTime ()
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

                for (int i = 0; i < numSamples; ++i)
                {
                    float headGains[2]; 

                    headGains[0] = playheads[0].gain();
                    headGains[1] = playheads[1].gain();

                    for (int chan = 0; chan < numChannels; ++chan)
                    {
                        delays[chan]->put(audio[chan][i]);

                        float out{ 0.0f };

                        for (int headIndex = 0; headIndex < 2; ++headIndex)
                        {
                            if (! playheads[headIndex].stopped())
                                out += delays[chan]->geti(playheads[headIndex].delayTime) * headGains[headIndex];
                        }

                        audio[chan][i] = out;
                    }
                }
			}
		};
	}
}
#endif  // DELAY_H_INCLUDED
