
/*
==============================================================================

Lfo.h
Created: 19 Apr 2017 12:36:33am
Author:  Jimmy

==============================================================================
*/

#ifndef SMOOTH_H_INCLUDED
#define SMOOTH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../LogUtil.h"

namespace Jimmy {
	namespace DSP {
		class SmoothFilter
		{
		public:
			SmoothFilter():
				mTaget(0.0), 
				mCurrent(0.0),
				mStep(0.0),
				mCount(0.0),
				mNumSamples(0)
			{
				
			}
			void preparePlay(float smoothingTimeInSec, int samplingRate) {
				mNumSamples = smoothingTimeInSec * samplingRate;
				mCount = 0;
				mCurrent = mTaget;
			}

			void setNewValue(float newValue) {
				if (mTaget != newValue) {
					mTaget = newValue;
					mCountDown = mNumSamples;

					if (mCountDown <= 0)
						mCurrent = mTaget;
					else
						mStep = (mTaget - mCurrent) / mCountDown;
				}
			}

			float getValue() {
				if (mCountDown <= 0)
					return mTaget;

				--mCountDown;
				mCurrent += mStep;
				return mCurrent;
			}
		private:
			int  mNumSamples, mCountDown;
			float mTaget, mCurrent, mStep, mCount;
		};
	};
};
#endif