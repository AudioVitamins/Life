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
namespace Jimmy {
	namespace DSP {
		class Vibrato {
			float mSampleRate;
			float mFrequency;
			float mDepth;
			float mFeedback;
			float mDelayMs;
			
			int mBufferSize;
			int mDelaySamples;

			int mNumChans;

			float mfDelaySamples;
			int mReadIdx;
			int mWriteIdx;

			LFO mLfo;
			AudioBuffer<float> mDelayBuffer;
		public:
			Vibrato(float sampleRate, int nChans) :
				mSampleRate(sampleRate),
				mNumChans(nChans),
				mFrequency(0.0),
				mDepth(0.0),
				mLfo(mSampleRate),
				mBufferSize(4 * mSampleRate),
				mDelayBuffer(mBufferSize, nChans),
				mReadIdx(0),
				mWriteIdx(0),
				mDelaySamples(0),
				mfDelaySamples(0.0)
			{
				mDelayBuffer.clear();
			}

			~Vibrato() {

			}

			void SetDelay(float delay0to1) {
				mDelayMs = delay0to1;
				mDelaySamples = mDelayMs* mSampleRate;
			};

			void SetFrequency(float frequency) {
				mLfo.SetFrequency(frequency);
			}

			void SetPhase(float phase) {
				mLfo.SetPhase(phase);
			}
			// Amount
			void SetDepth(float depth) {
				mDepth = depth;
			};

			void SetFeedback(float feedBackPct) {
				mFeedback = feedBackPct;
			};

			void process(AudioBuffer<float> &buffer) {
				const float **input = buffer.getArrayOfReadPointers();
				float **output = buffer.getArrayOfWritePointers();
				float **delayBuffer = mDelayBuffer.getArrayOfWritePointers();
				int numSamples = buffer.getNumSamples();
				for (int i = 0; i < numSamples; i++) {
					float lfo = mLfo.Value();
					float offset = lfo * mDepth / 200 * mDelaySamples;
					mReadIdx = mWriteIdx - (int)(offset);

					int nReadIndex_1 = mReadIdx - 1;
					if (nReadIndex_1 < 0) {
						nReadIndex_1 = mBufferSize - 1; // m_nBufferSize-1 is last location
					}

					// Readidx check boundary
					if (mReadIdx < 0) {
						mReadIdx += mBufferSize;
					}

					float fFracDelay = offset - (int)offset;

					for (int c = 0; c < mNumChans; c++) {
						float xn = input[c][i];
						float &yout = output[c][i];
						if (mReadIdx == mWriteIdx && offset < 1.0) {
							yout = xn;
						}
						else {
							
							float yn = delayBuffer[c][mReadIdx];
							float yn_1 = delayBuffer[c][nReadIndex_1];
							float fInterp = dLinTerp(0, 1, yn, yn_1, fFracDelay);
							yout = fInterp;
						}

						delayBuffer[c][mWriteIdx] = xn + mFeedback /100.0 * yout;
					}
					// Incr Write index
					mWriteIdx++;
					if (mWriteIdx >= mBufferSize)
						mWriteIdx = 0;
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
		};
	}
}



#endif  // VIBRATO_H_INCLUDED
