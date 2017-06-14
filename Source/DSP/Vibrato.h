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
		class Vibrato {
			float mSampleRate;
			float mFrequency;
			float mDepth;
			float mFeedback;
			float mDelayMs;
			
			int mBufferSize;
			//int mDelaySamples;
			
			int mDelaySamplesForVibrato;
			int mNumChans;

			float mfDelaySamples;
			Array<int> mReadIdx;
			Array<int> mWriteIdx;

			Array<LFO> mLfo;
			//int writeIndex[2] = { 0,0 };
			//float phase[2] = { 0,0 };
			AudioBuffer<float> mDelayBuffer;

			Array<SmoothFilter> mSmoothFeedback;
		public:
			Vibrato(float sampleRate, int nChans) :
				mSampleRate(sampleRate),
				mNumChans(nChans),
				mFrequency(0.0),
				mDepth(0.0),
				mLfo(),
				mBufferSize(2 * mSampleRate),
				mReadIdx(),
				mWriteIdx(),
				//mDelaySamples(0),
				mDelaySamplesForVibrato(2.0f /1000.0f * mSampleRate),
				mDelayBuffer(),
				mfDelaySamples(0.0)
			{
				for (int c = 0; c < mNumChans; c++) {
					mLfo.add(LFO(mSampleRate));
					mReadIdx.add(0);
					mWriteIdx.add(0);
				}
				mDelayBuffer.setSize(nChans, mBufferSize);
				mDelayBuffer.clear();
				for (int c = 0; c < mNumChans; c++) {
					mSmoothFeedback.add(SmoothFilter());
				}
			}

			~Vibrato() {

			}

			void SetFrequency(float frequency) {
				for (int c = 0; c < mNumChans; c++) {
					mLfo.getRawDataPointer()[c].SetFrequency(frequency);
				}
			}

			void preparePlay(float sampleRate) {
				//JIMMY_LOGGER_ACTIVATE(JIMMY_LOGGER_INFO);
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

			// Amount
			void SetDepth(float depth) {
				mDepth = depth;
			};

			void SetFeedback(float feedBackPct) {
				//mFeedback = feedBackPct;
				for (int c = 0; c < mNumChans; c++) {
					mSmoothFeedback.getReference(c).setNewValue(feedBackPct);
				}
			};

			void process(AudioBuffer<float> &buffer) {
				int numSamples = buffer.getNumSamples();

				//AudioBuffer<float> output(mNumChans, numSamples);
				//output.clear();
				//float **outputBuffer = mDelayBuffer.getArrayOfWritePointers();
				/*float depthCopy = round(0.001*mSampleRate);
				float delay = round(0.001*mSampleRate);
	
				float deltaAngle = updateAngle(8);
				for (int channel = 0; channel < mNumChans; ++channel)
				{
					int pos = writeIndex[channel];
					float* channelData = buffer.getWritePointer(channel);

					for (int i = 0; i < numSamples; i++)
					{
						delayBuffer[channel][pos] = channelData[i];

						double modfreq = sin(phase[channel]);

						phase[channel] = phase[channel] + deltaAngle;

						if (phase[channel] > float_Pi * 2)
							phase[channel] = phase[channel] - (2 * float_Pi);


						float tap = 1 + delay + depthCopy * modfreq;
						int n = floor(tap);

						float frac = tap - n;

						int rindex = floor(pos - n);

						if (rindex < 0)
							rindex = rindex + mDelaySamplesForVibrato;

						float sample = 0;
						float y_1 = 0.0f, yn = 0.0f;
						if (rindex == 0) {
							y_1 = delayBuffer[channel][mDelaySamplesForVibrato - 1];
							yn = delayBuffer[channel][rindex];
							sample += y_1 *frac + (1 - frac)*yn;
						}
						else {
							y_1 = delayBuffer[channel][rindex - 1];
							yn = delayBuffer[channel][rindex];
							sample += y_1 *frac + (1 - frac)*yn;
						}
						//JIMMY_LOGGER_PRINT(JIMMY_LOGGER_INFO, "Chan %d Read %d Write %d Mode %1.7f Offset %3.7f yn-1 %3.7f yn %3.7f out %3.7f\n",
						//	channel, rindex, pos, modfreq, tap, y_1, yn, sample);
						pos = (pos + 1) % mDelaySamplesForVibrato;

						channelData[i] = sample;
						//channelData[i] = dry*channelData[i] + wet*sample;
					}
					writeIndex[channel] = pos;
				}*/

				for (int c = 0; c < mNumChans; c++) {

					const float *inputBuffer = buffer.getReadPointer(c);
					float *outputBuffer = buffer.getWritePointer(c);
					float *delayBuffer = mDelayBuffer.getWritePointer(c);

					LFO &lfo = mLfo.getRawDataPointer()[c];
					int &writeIdx = mWriteIdx.getRawDataPointer()[c];
					
					SmoothFilter &smooth = mSmoothFeedback.getReference(c);
					for (int i = 0; i < numSamples; i++) {

 						float modFreq = lfo.Value();
						int delay = floor( mDepth / 100.0f * mDelaySamplesForVibrato);
						float offset = 1 + delay +  modFreq * delay;
						int readIdx = (writeIdx - (int)floor(offset) + mBufferSize) % mBufferSize;
						int nReadIndex_1 = (readIdx - 1 + mBufferSize) % mBufferSize;
						float frac = offset - floor(offset);

						float xn = inputBuffer[i];
						float yn = delayBuffer[readIdx];
						float yn_1 = delayBuffer[nReadIndex_1];
						float fInterp = yn_1 * frac + (1 - frac)* yn;
						outputBuffer[i] = fInterp;
						//JIMMY_LOGGER_PRINT(INFORMATION, "Chan %d Read %d Write %d Delta %d Mode Freq %1.7f Offset %3.7f yn-1 %3.7f yn %3.7f delta %3.7f out %3.7f\n",
						//c, readIdx, writeIdx, writeIdx - readIdx, modFreq, offset, yn_1, yn, yn_1 - yn, fInterp);
						mFeedback = smooth.getValue();
						delayBuffer[writeIdx] = (1.0 - mFeedback / 100.0) * xn + mFeedback /100.0 * fInterp;
						// Incr Write index
						writeIdx = (writeIdx + 1) % mBufferSize;
					}
					
					
				}
				//float *delayBuffer = mDelayBuffer.getWritePointer(0);
				//int sam = mDelayBuffer.getNumSamples();
				//for (int i = 0; i < sam; i++) {
				//JIMMY_LOGGER_PRINT(INFORMATION, "Idx %d  %3.5f\n", i,
				//delayBuffer[i]);
				//}
				////buffer.copyFrom();
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
