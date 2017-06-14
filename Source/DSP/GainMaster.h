/*
  ==============================================================================

    GainMaster.h
    Created: 3 May 2017 11:51:41pm
    Author:  Jimmy

  ==============================================================================
*/

#ifndef GAINMASTER_H_INCLUDED
#define GAINMASTER_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
class SmoothFilter
{
public:
	SmoothFilter(float smoothingTimeInSec, int samplingRate)
	{
		m_f_smoothNess = exp(-2.0 *float_Pi / (smoothingTimeInSec * samplingRate));
		m_f_alpha = 1.0f - m_f_smoothNess;
		m_f_lastestOut = 0.0f;
	}
	SmoothFilter(float a, float b) {
		m_f_alpha = a;
		m_f_smoothNess = b;
		m_f_lastestOut = 0.0f;
	}
	SmoothFilter(float smoothNess) {
		m_f_smoothNess = smoothNess;
		m_f_alpha = 1.f - m_f_smoothNess;
		m_f_lastestOut = 0;
	}
	SmoothFilter() {
		m_f_smoothNess = 0.99f;
		m_f_alpha = 1.f - m_f_smoothNess;
		m_f_lastestOut = 0;
	};
	~SmoothFilter() {};

	inline float Process(float in)
	{
		//m_f_lasterOut = in * (1.f - m_f_smoothNess) + m_f_lasterOut * m_f_smoothNess; 
		// = in - a * in + y[n -1] * a
		// = in - (in - y[n -1]) *a
		m_f_lastestOut = (in * m_f_alpha) + (m_f_lastestOut * m_f_smoothNess);
		return m_f_lastestOut;
	}
private:
	float m_f_alpha, m_f_smoothNess, m_f_lastestOut;
};
class GainMaster {
	enum {
		GAINDB
		,GAIN0TO1
	};
	int mMode;
	float mGain;
	
	int mNumChans;
	Array<SmoothFilter> mSmooth;
	ScopedPointer<NormalisableRange<float>> mNormalize;
public:
	GainMaster(int numChans): mNumChans(numChans){
		mMode = GAINDB;
		for(int i = 0; i < numChans; i++)
		 mSmooth.add(SmoothFilter(1.0f, 0.01f));
	};
	GainMaster(float min, float max, int numChans): mNumChans(numChans) {
		mMode = GAIN0TO1;
		mNormalize = new NormalisableRange<float>(min, max);
		for (int i = 0; i < numChans; i++)
			mSmooth.add(SmoothFilter(1.0f, 0.01f));
	};

	~GainMaster(){
		mNormalize = nullptr;
		mSmooth.clear();
	};
	
	void SetGainDB(float gainDb) {
		mGain = dbToGain(gainDb);
	};

	void SetGainDb0to1(float gainDB0to1) {
		mGain = dbToGain(mNormalize->convertFrom0to1(gainDB0to1));
	};

	void process(const float *input, float *output, int chan) {

	}
private:
	float dbToGain(float db) {
		return Decibels::decibelsToGain<float>(db);
	};

	float smoothGain(float gain) {
	};
};



#endif  // GAINMASTER_H_INCLUDED
