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
#include "DSP/LRtoMSConverter.h"
#include "Config.h"
#include "./authorization/EditorState.h"

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

	bool ProcessMS = false;
	bool InvertVibrato = false;
	bool InvertTremolo = false;
//	bool DelayLink = false;
//	bool FeedbackLink = false;
    
    //@AS
    bool mUnlocked;
    EditorState mEditorState;

private:
    //==============================================================================

//	ScopedPointer<Jimmy::DSP::StaticDelay> mDelay;
//	ScopedPointer<Jimmy::DSP::Vibrato> mVibrato;

	ScopedPointer<LRtoMSConverter> mMSConverter;
	
	ScopedPointer<Jimmy::DSP::DelayVibrato> mDelayVibrato[2];
	ScopedPointer<Jimmy::DSP::Tremolo> mTremolo[2];

	ScopedPointer<Jimmy::DSP::IIRFilterHP> mFilterHP[2];
	ScopedPointer<Jimmy::DSP::IIRFilterLP> mFilterLP[2];

	ScopedPointer<Jimmy::DSP::Width> mWidth;
	ScopedPointer<Jimmy::DSP::WetDry> mWet;

	ScopedPointer<Jimmy::DSP::GainMaster> mGainMaster;
	
	float RateToFrequency(float rate) const;

    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
	ScopedPointer<AudioProcessorValueTreeState> mState;
	ScopedPointer<UndoManager>                  mUndoManager;

	static String paramDelayLeft;
	static String paramDelayRight;

	static String paramPitchRateLeft;
	static String paramPitchRateRight;

	static String paramPitchAmountLeft;
	static String paramPitchAmountRight;

	static String paramFeedbackLeft;
	static String paramFeedbackRight;

	static String paramAmplitudeRateLeft;
	static String paramAmplitudeRateRight;

	static String paramAmplitudeAmountLeft;
	static String paramAmplitudeAmountRight;


	static String paramHighFreqLeft;
	static String paramHighFreqRight;
	static String paramLowFreqLeft;
	static String paramLowFreqRight;

	static String paramWidth;
	static String paramWetDry;

	static String paramGainMaster;

	static String paramLR_Or_MSToggle;
	static String paramPitchOscSyncToggle;
	static String paramAmpOscSyncToggle;

	static String paramDelayLinkToggle;
	static String paramFeedbackLinkToggle;

	double lastKnownBpm{ 0.0 };
	AudioSampleBuffer dryAudioBuffer;
	AudioSampleBuffer SideBuffer;

	void parameterChanged(const String& parameterID, float newValue) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LifeAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
