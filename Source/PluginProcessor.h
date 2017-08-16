/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "DSP/Delay.h"
#include "DSP/Vibrato.h"
#include "DSP/Tremolo.h"
#include "DSP/IIRFilter.h"
#include "DSP/Width.h"
#include "DSP/WetDry.h"
#include "DSP/GainMaster.h"
#include "Config.h"

//==============================================================================
/**
*/
class LifeAudioProcessor : public AudioProcessor,
	public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    LifeAudioProcessor();
    ~LifeAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================

//	ScopedPointer<Jimmy::DSP::StaticDelay> mDelay;
//	ScopedPointer<Jimmy::DSP::Vibrato> mVibrato;

	ScopedPointer<Jimmy::DSP::DelayVibrato> mDelayVibrato;
	ScopedPointer<Jimmy::DSP::Tremolo> mTremolo;

	ScopedPointer<Jimmy::DSP::IIRFilterHP> mFilterHP;
	ScopedPointer<Jimmy::DSP::IIRFilterLP> mFilterLP;

	ScopedPointer<Jimmy::DSP::Width> mWidth;
	ScopedPointer<Jimmy::DSP::WetDry> mWet;

	ScopedPointer<Jimmy::DSP::GainMaster> mGainMaster;


	float RateToFrequency(float rate) const;

    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
	ScopedPointer<AudioProcessorValueTreeState> mState;
	ScopedPointer<UndoManager>                  mUndoManager;

	static String paramDelay;
	
	static String paramPitchRate;
	static String paramPitchAmount;
	static String paramFeedback;

	static String paramAmplitudeRate;
	static String paramAmplitudeAmount;

	static String paramHighFreq;
	static String paramLowFreq;

	static String paramWidth;
	static String paramWetDry;

	static String paramGainMaster;

	double lastKnownBpm{ 0.0 };
	AudioSampleBuffer dryAudioBuffer;

	void parameterChanged(const String& parameterID, float newValue) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LifeAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
