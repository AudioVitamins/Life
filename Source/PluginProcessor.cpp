/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LogUtil.h"
String LifeAudioProcessor::paramDelayLeft = "DelayLeft";
String LifeAudioProcessor::paramDelayRight = "DelayRight";

String LifeAudioProcessor::paramPitchRateLeft = "PitchRateLeft";
String LifeAudioProcessor::paramPitchAmountLeft = "PitchAmountLeft";
String LifeAudioProcessor::paramPitchRateRight = "PitchRateRight";
String LifeAudioProcessor::paramPitchAmountRight = "PitchAmountRight";

String LifeAudioProcessor::paramFeedbackLeft = "Feedback Left";
String LifeAudioProcessor::paramFeedbackRight = "Feedback Right";

String LifeAudioProcessor::paramAmplitudeRateLeft = "AmplitudeRateLeft";
String LifeAudioProcessor::paramAmplitudeRateRight = "AmplitudeRateRight";

String LifeAudioProcessor::paramAmplitudeAmountLeft = "AmplitudeAmountLeft";
String LifeAudioProcessor::paramAmplitudeAmountRight = "AmplitudeAmountRight";

String LifeAudioProcessor::paramHighFreqLeft = "HighFreqLeft";
String LifeAudioProcessor::paramHighFreqRight = "HighFreqRight";
String LifeAudioProcessor::paramLowFreqLeft = "LowFreqLeft";
String LifeAudioProcessor::paramLowFreqRight = "LowFreqRight";

String LifeAudioProcessor::paramWidth = "Width";
String LifeAudioProcessor::paramWetDry = "WetDry";

String LifeAudioProcessor::paramGainMaster = "GainMaster";

String LifeAudioProcessor::paramLR_Or_MSToggle = "LR or MS";
String LifeAudioProcessor::paramPitchOscSyncToggle = "Invert Vibrato";
String LifeAudioProcessor::paramAmpOscSyncToggle = "Invert Tremolo";

String LifeAudioProcessor::paramDelayLinkToggle = "Delay Link";
String LifeAudioProcessor::paramFeedbackLinkToggle = "Feedback Link";

//==============================================================================
LifeAudioProcessor::LifeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	
	mUndoManager = new UndoManager();
	mState = new AudioProcessorValueTreeState(*this, mUndoManager);
	// Current design no need add listener 
	mState->createAndAddParameter(paramDelayLeft, "DelayLeft", TRANS("DelayLeft"), NormalisableRange<float>(0.0f, 100.0f, 0.01), 0.0, nullptr, nullptr);
	mState->createAndAddParameter(paramDelayRight, "DelayRight", TRANS("DelayRight"), NormalisableRange<float>(0.0f, 100.0f, 0.01), 0.0, nullptr, nullptr);
	
	mState->createAndAddParameter(paramPitchRateLeft, "Pitch Rate Left", TRANS("Pitch Rate Left"), NormalisableRange<float>(1.0f, 4.0f, 1), 1.0, nullptr, nullptr);
	mState->createAndAddParameter(paramPitchRateRight, "Pitch Rate Right", TRANS("Pitch Rate Right"), NormalisableRange<float>(1.0f, 4.0f, 1), 1.0, nullptr, nullptr);

	mState->createAndAddParameter(paramPitchAmountLeft, "Pitch Amount Left", TRANS("Pitch Amount Left"), NormalisableRange<float>(0.0f, 5.0f, 1), 0.0, nullptr, nullptr);
	mState->createAndAddParameter(paramPitchAmountRight, "Pitch Amount Right", TRANS("Pitch Amount Right"), NormalisableRange<float>(0.0f, 5.0f, 1), 0.0, nullptr, nullptr);
	
	mState->createAndAddParameter(paramFeedbackLeft, "Feedback Left", TRANS("Feedback Left"), NormalisableRange<float>(0.0f, 100.0f, 0.1), 50.0, nullptr, nullptr);
	mState->createAndAddParameter(paramFeedbackRight, "Feedback Right", TRANS("Feedback Right"), NormalisableRange<float>(0.0f, 100.0f, 0.1), 50.0, nullptr, nullptr);

	mState->createAndAddParameter(paramAmplitudeRateLeft, "Amplitude Rate Left", TRANS("Amplitude Rate Left"), NormalisableRange<float>(1.0f, 4.0f, 1), 1.0, nullptr, nullptr);
	mState->createAndAddParameter(paramAmplitudeRateRight, "Amplitude Rate Right", TRANS("Amplitude Rate Right"), NormalisableRange<float>(1.0f, 4.0f, 1), 1.0, nullptr, nullptr);

	mState->createAndAddParameter(paramAmplitudeAmountLeft, "Amplitude Amount Left", TRANS("Amplitude Amount Left"), NormalisableRange<float>(0.0f, 5.0f, 1), 0.0, nullptr, nullptr);
	mState->createAndAddParameter(paramAmplitudeAmountRight, "Amplitude Amount Right", TRANS("Amplitude Amount Right"), NormalisableRange<float>(0.0f, 5.0f, 1), 0.0, nullptr, nullptr);
	
	mState->createAndAddParameter(paramHighFreqLeft, "High Freq Left", TRANS("High Freq Left"), NormalisableRange<float>(20.0f, 20000.0f, 0.1), 20.0, nullptr, nullptr);
	mState->createAndAddParameter(paramHighFreqRight, "High Freq Right", TRANS("High Freq Right"), NormalisableRange<float>(20.0f, 20000.0f, 0.1), 20.0, nullptr, nullptr);

	mState->createAndAddParameter(paramLowFreqLeft, "Low Freq Left", TRANS("Low Freq Left"), NormalisableRange<float>(20.0f, 20000.0f, 0.1), 20000.0, nullptr, nullptr);
	mState->createAndAddParameter(paramLowFreqRight, "Low Freq Right", TRANS("Low Freq Right"), NormalisableRange<float>(20.0f, 20000.0f, 0.1), 20000.0, nullptr, nullptr);
	
	mState->createAndAddParameter(paramWidth, "Width", TRANS("Width"), NormalisableRange<float>(0.0f, 2.0f, 0.01), 1.0, nullptr, nullptr);
	mState->createAndAddParameter(paramWetDry, "Wet Dry", TRANS("Wet Dry"), NormalisableRange<float>(0.0f, 100.0f, 0.1), 50.0, nullptr, nullptr);

	mState->createAndAddParameter(paramGainMaster, "Gain Master", TRANS("Gain Master"), NormalisableRange<float>(-10.0f, 10.0f, 0.1), 0.0, nullptr, nullptr);

	mState->createAndAddParameter(paramLR_Or_MSToggle, "LR or MS Toggle", TRANS("LR or MS Toggle"), NormalisableRange<float>(0, 1, 1), 0, nullptr, nullptr);
	mState->createAndAddParameter(paramPitchOscSyncToggle, "Invert Vibrato", TRANS("Invert Vibrato"), NormalisableRange<float>(0, 1, 1), 0, nullptr, nullptr);
	mState->createAndAddParameter(paramAmpOscSyncToggle, "Invert Tremolo", TRANS("Invert Tremolo"), NormalisableRange<float>(0, 1, 1), 0, nullptr, nullptr);

	mState->createAndAddParameter(paramDelayLinkToggle, "Delay Link", TRANS("Delay Link"), NormalisableRange<float>(0, 1, 1), 0, nullptr, nullptr);
	mState->createAndAddParameter(paramFeedbackLinkToggle, "Feedback Link", TRANS("Feedback Link"), NormalisableRange<float>(0, 1, 1), 0, nullptr, nullptr);

	mState->state = ValueTree("LifeParameters");

	mState->addParameterListener(paramDelayLeft, this);
	mState->addParameterListener(paramDelayRight, this);

	mState->addParameterListener(paramPitchRateLeft, this);
	mState->addParameterListener(paramPitchRateRight, this);
	mState->addParameterListener(paramPitchAmountLeft, this);
	mState->addParameterListener(paramPitchAmountRight, this);

	mState->addParameterListener(paramFeedbackLeft, this);
	mState->addParameterListener(paramFeedbackRight, this);

	mState->addParameterListener(paramAmplitudeRateLeft, this);
	mState->addParameterListener(paramAmplitudeRateRight, this);

	mState->addParameterListener(paramAmplitudeAmountLeft, this);
	mState->addParameterListener(paramAmplitudeAmountRight, this);
	
	mState->addParameterListener(paramAmplitudeAmountLeft, this);
	mState->addParameterListener(paramAmplitudeAmountRight, this);

	mState->addParameterListener(paramLowFreqLeft, this);
	mState->addParameterListener(paramLowFreqRight, this);

	mState->addParameterListener(paramHighFreqLeft, this);
	mState->addParameterListener(paramHighFreqRight, this);
		
	mState->addParameterListener(paramWidth, this);
	mState->addParameterListener(paramWetDry, this);

	mState->addParameterListener(paramGainMaster, this);

	mState->addParameterListener(paramLR_Or_MSToggle, this);
	mState->addParameterListener(paramPitchOscSyncToggle, this);
	mState->addParameterListener(paramAmpOscSyncToggle, this);

	mState->addParameterListener(paramDelayLinkToggle, this);
	mState->addParameterListener(paramFeedbackLinkToggle, this);

    float sampleRate = 44100.0;

    int numOutputChannel = getTotalNumOutputChannels(); 
    mDelayVibrato[L] = new Jimmy::DSP::DelayVibrato(float(sampleRate), 0.1f, numOutputChannel);
	mDelayVibrato[R] = new Jimmy::DSP::DelayVibrato(float(sampleRate), 0.1f, numOutputChannel);

    mTremolo[L] = new Jimmy::DSP::Tremolo(float(sampleRate), numOutputChannel);
	mTremolo[R] = new Jimmy::DSP::Tremolo(float(sampleRate), numOutputChannel);
	
    mFilterHP[L] = new Jimmy::DSP::IIRFilterHP(float(sampleRate), numOutputChannel);
	mFilterHP[R] = new Jimmy::DSP::IIRFilterHP(float(sampleRate), numOutputChannel);

    mFilterLP[L] = new Jimmy::DSP::IIRFilterLP(float(sampleRate), numOutputChannel);
	mFilterLP[R] = new Jimmy::DSP::IIRFilterLP(float(sampleRate), numOutputChannel);

    mWidth = new Jimmy::DSP::Width();
    mWet = new Jimmy::DSP::WetDry();
    
    mGainMaster = new Jimmy::DSP::GainMaster(-10.0f, 10.0f, numOutputChannel);
}

LifeAudioProcessor::~LifeAudioProcessor()
{
}

//==============================================================================
const String LifeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LifeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LifeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double LifeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LifeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LifeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LifeAudioProcessor::setCurrentProgram (int index)
{
}

const String LifeAudioProcessor::getProgramName (int index)
{
    return String();
}

void LifeAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LifeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	int numOutputChannel = getTotalNumOutputChannels();
    
	dryAudioBuffer.setSize(numOutputChannel, samplesPerBlock, false, true);
	SideBuffer.setSize(numOutputChannel, samplesPerBlock, false, true);

	mDelayVibrato[L] = new Jimmy::DSP::DelayVibrato(float(sampleRate), 0.1f, numOutputChannel);
	mDelayVibrato[R] = new Jimmy::DSP::DelayVibrato(float(sampleRate), 0.1f, numOutputChannel);

	mTremolo[L] = new Jimmy::DSP::Tremolo(float(sampleRate), numOutputChannel);
	mTremolo[R] = new Jimmy::DSP::Tremolo(float(sampleRate), numOutputChannel);
	
	mFilterHP[L] = new Jimmy::DSP::IIRFilterHP(float(sampleRate), numOutputChannel);
	mFilterHP[R] = new Jimmy::DSP::IIRFilterHP(float(sampleRate), numOutputChannel);

	mFilterLP[L] = new Jimmy::DSP::IIRFilterLP(float(sampleRate), numOutputChannel);
	mFilterLP[R] = new Jimmy::DSP::IIRFilterLP(float(sampleRate), numOutputChannel);

	mWidth = new Jimmy::DSP::Width();
	mWet = new Jimmy::DSP::WetDry();
    
    mGainMaster = new Jimmy::DSP::GainMaster(-10.0f, 10.0f, numOutputChannel);
    
	//mDelay = new Jimmy::DSP::StaticDelay(float(sampleRate), 0.1f, numOutputChannel);
	// Aplly Pitch/Feedback
	mDelayVibrato[L]->preparePlay(sampleRate, samplesPerBlock);
	mDelayVibrato[R]->preparePlay(sampleRate, samplesPerBlock);
	float *delayMsLeft = mState->getRawParameterValue(paramDelayLeft);
	float *delayMsRight = mState->getRawParameterValue(paramDelayRight);

	mDelayVibrato[L]->setDelayInMiliSec(*delayMsLeft);
	mDelayVibrato[R]->setDelayInMiliSec(*delayMsRight);
	//mDelay->preparePlay(samplesPerBlock);
	//mDelay->setDelayInMiliSec(*delayMs);

	float *ratePitchLeft = mState->getRawParameterValue(paramPitchRateLeft);
	float *amountPitchLeft = mState->getRawParameterValue(paramPitchAmountLeft);
	float *ratePitchRight = mState->getRawParameterValue(paramPitchRateRight);
	float *amountPitchRight = mState->getRawParameterValue(paramPitchAmountRight);

	float *feedbackLeft = mState->getRawParameterValue(paramFeedbackLeft);
	float *feedbackRight = mState->getRawParameterValue(paramFeedbackRight);
	
	float freqPitchLeft = RateToFrequency(*ratePitchLeft);
	float freqPitchRight = RateToFrequency(*ratePitchRight);

	//mVibrato->preparePlay(sampleRate);
//	mVibrato->SetFrequency(freqPitch);
//	mVibrato->SetDepth(*amountPitch);
//	mVibrato->SetFeedback(*feedback);
	mDelayVibrato[L]->SetFrequency(freqPitchLeft);
	mDelayVibrato[R]->SetFrequency(freqPitchRight);

	mDelayVibrato[L]->SetDepth(*amountPitchLeft);
	mDelayVibrato[R]->SetDepth(*amountPitchRight);

	mDelayVibrato[L]->SetFeedback(*feedbackLeft);
	mDelayVibrato[R]->SetFeedback(*feedbackRight);
	
	// Apply AM
	float *rateAmLeft = mState->getRawParameterValue(paramAmplitudeRateLeft);
	float *rateAmRight = mState->getRawParameterValue(paramAmplitudeRateRight);

	float freqAmLeft = RateToFrequency(*rateAmLeft);
	float freqAmRight = RateToFrequency(*rateAmRight);
		
	float *amountAmLeft = mState->getRawParameterValue(paramAmplitudeAmountLeft);
	float *amountAmRight = mState->getRawParameterValue(paramAmplitudeAmountRight);

	mTremolo[L]->SetFrequency(freqAmLeft);
	mTremolo[R]->SetFrequency(freqAmRight);

	mTremolo[L]->SetDepth(*amountAmLeft);
	mTremolo[R]->SetDepth(*amountAmRight);

	// Apply Filter
	float *highFreqLeft = mState->getRawParameterValue(paramHighFreqLeft);
	float *highFreqRight = mState->getRawParameterValue(paramHighFreqRight);

	mFilterHP[L]->changeCutOff(*highFreqLeft);
	mFilterHP[R]->changeCutOff(*highFreqRight);

	float *lowFreqLeft = mState->getRawParameterValue(paramLowFreqLeft);
	float *lowFreqRight = mState->getRawParameterValue(paramLowFreqRight);

	mFilterLP[L]->changeCutOff(*lowFreqLeft);
	mFilterLP[R]->changeCutOff(*lowFreqRight);

	//Apply Width
	float width0to1 = mState->getParameter(paramWidth)->getValue();
	mWidth->preparePlay(sampleRate);
	mWidth->setWidth(width0to1);

	//Apply Dry/Wet
	float wet0to1 = mState->getParameter(paramWetDry)->getValue();
	mWet->preparePlay(sampleRate);
	mWet->setWet(wet0to1);

	// Master Gain
	float gain0to1 = mState->getParameter(paramGainMaster)->getValue();
	mGainMaster->preparePlay(sampleRate);
	mGainMaster->SetGainDb0to1(gain0to1);

}

void LifeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LifeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LifeAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	if (getPlayHead()->getCurrentPosition(currentPositionInfo))
		lastKnownBpm = currentPositionInfo.bpm;

	if (lastKnownBpm == 0.0)
		lastKnownBpm = 120.0;

	const int totalNumInputChannels = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();

	// Clear/Reset apart of output chans
	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	if (totalNumInputChannels < 1 || totalNumInputChannels > 2 || buffer.getNumSamples() == 0)
		return;
	
    float *ratePitchLeft = mState->getRawParameterValue(paramPitchRateLeft);
	float *ratePitchRight = mState->getRawParameterValue(paramPitchRateRight);

	float *delayMsLeft = mState->getRawParameterValue(paramDelayLeft);
	float *delayMsRight = mState->getRawParameterValue(paramDelayRight);

    float freqLeft = RateToFrequency(*ratePitchLeft);
	float freqRight = RateToFrequency(*ratePitchRight);
				
    mDelayVibrato[L]->SetFrequency(freqLeft);
	mDelayVibrato[R]->SetFrequency(freqRight);

	mDelayVibrato[L]->setDelayInMiliSec(*delayMsLeft);
	mDelayVibrato[R]->setDelayInMiliSec(*delayMsRight);
		
	dryAudioBuffer.makeCopyOf(buffer, true);
	SideBuffer.makeCopyOf(buffer, true);
	
	if (ProcessMS == true)
	{
		mMSConverter->ConvertLRToMid(buffer);
		mMSConverter->ConvertLRToSide(buffer, SideBuffer);
	}

	mDelayVibrato[L]->process(buffer, L);
	mDelayVibrato[R]->process(buffer, R);

	mTremolo[L]->process(buffer, L);
	mTremolo[R]->process(buffer, R);
	
	mFilterHP[L]->process(buffer, L);
	mFilterHP[R]->process(buffer, R);

	mFilterLP[L]->process(buffer, L);
	mFilterLP[R]->process(buffer, R);
		
	if (ProcessMS == true) { mMSConverter->ConvertMSToLR(buffer); }

	mWidth->process(buffer);

	mWet->process(dryAudioBuffer, buffer);

	mGainMaster->process(buffer);
}

//==============================================================================

void LifeAudioProcessor::parameterChanged(const String& parameterID, float newValue) 
{
    //if (mDelay == nullptr)
    //{
    //    /* There's a right can of worms here ... I think the approach for handling parameter
    //     * changes with this listener might be wrong.   Anyway - it's possible that this
    //     * is called before prepareToPlay which causes a crash unless we do this check. 
    //     * 
    //     * Also getPlayHead() is being called somewhere else during prepareToPlay and this isn't
    //     * a valid place to call it - you should only call that in processBlock...
    //     */
    //    return; 
    //}

	if (parameterID == LifeAudioProcessor::paramDelayLeft) {
//		suspendProcessing(true);
		//mDelay->setDelayInMiliSec(newValue);
		mDelayVibrato[L]->setDelayInMiliSec(newValue);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramDelayRight) {
		//		suspendProcessing(true);
		//mDelay->setDelayInMiliSec(newValue);
		mDelayVibrato[R]->setDelayInMiliSec(newValue);
		//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramPitchRateLeft) {
//		suspendProcessing(true);AU
		//float *ratePitch = mState->getRawParameterValue(paramPitchRate);
		float freqPitch = RateToFrequency(newValue);
		mDelayVibrato[L]->SetFrequency(freqPitch);
		//mVibrato->SetFrequency(freqPitch);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramPitchRateRight) {
		//		suspendProcessing(true);AU
		//float *ratePitch = mState->getRawParameterValue(paramPitchRate);
		float freqPitch = RateToFrequency(newValue);
		mDelayVibrato[R]->SetFrequency(freqPitch);

		//mVibrato->SetFrequency(freqPitch);
		//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramPitchAmountLeft) {
//		suspendProcessing(true);
		//float *amountPitch = mState->getRawParameterValue(paramPitchAmount);
		//mVibrato->SetDepth(newValue);
		mDelayVibrato[L]->SetDepth(newValue);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramPitchAmountRight) {
		//		suspendProcessing(true);
		//float *amountPitch = mState->getRawParameterValue(paramPitchAmount);
		//mVibrato->SetDepth(newValue);
		mDelayVibrato[R]->SetDepth(newValue);
		//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramFeedbackLeft) {
//		suspendProcessing(true);
		//float *feedback = mState->getRawParameterValue(paramFeedback);
		//mVibrato->SetFeedback(newValue);
		mDelayVibrato[L]->SetFeedback(newValue);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramFeedbackRight) {
		//		suspendProcessing(true);
		//float *feedback = mState->getRawParameterValue(paramFeedback);
		//mVibrato->SetFeedback(newValue);
		mDelayVibrato[R]->SetFeedback(newValue);
		//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramAmplitudeRateLeft) {
//		suspendProcessing(true);
		//float *rateAm = mState->getRawParameterValue(paramAmplitudeRate);
		float freqAm = RateToFrequency(newValue);
		mTremolo[L]->SetFrequency(freqAm);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramAmplitudeRateRight) {
		//		suspendProcessing(true);
		//float *rateAm = mState->getRawParameterValue(paramAmplitudeRate);
		float freqAm = RateToFrequency(newValue);
		mTremolo[R]->SetFrequency(freqAm);
		//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramAmplitudeAmountLeft) {
//		suspendProcessing(true);
		//float *amountAm = mState->getRawParameterValue(paramAmplitudeAmount);
		mTremolo[L]->SetDepth(newValue);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramAmplitudeAmountRight) {
		//		suspendProcessing(true);
		//float *amountAm = mState->getRawParameterValue(paramAmplitudeAmount);
		mTremolo[R]->SetDepth(newValue);
		//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramHighFreqLeft) {
		// Apply Filter
//		suspendProcessing(true);
		mFilterHP[L]->changeCutOff(newValue);
//		mFilterHP->reset();
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramHighFreqRight) {
		// Apply Filter
		//		suspendProcessing(true);
		mFilterHP[R]->changeCutOff(newValue);
		//		mFilterHP->reset();
		//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramLowFreqLeft) {
		// Apply Filter
//		suspendProcessing(true);
		mFilterLP[L]->changeCutOff(newValue);
//		mFilterLP->reset();
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramLowFreqRight) {
		// Apply Filter
		//		suspendProcessing(true);
		mFilterLP[R]->changeCutOff(newValue);
		//		mFilterLP->reset();
		//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramWidth) {
//		suspendProcessing(true);
		float width0to1 = mState->getParameterRange(paramWidth).convertTo0to1(newValue);
		mWidth->setWidth(width0to1);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramWetDry) {
//		suspendProcessing(true);
		float wet0to1 = mState->getParameterRange(paramWetDry).convertTo0to1(newValue);
		mWet->setWet(wet0to1);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramGainMaster) {
//		suspendProcessing(true);
		mGainMaster->SetGainDB(newValue);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramLR_Or_MSToggle)
	{
		ProcessMS = newValue;
	}
	else if (parameterID == LifeAudioProcessor::paramPitchOscSyncToggle)
	{
		InvertVibrato = newValue;
		if (InvertVibrato == true)
		{
			mDelayVibrato[L]->SetPhase(0.0);
			mDelayVibrato[R]->SetPhase(float_Pi);
		}
		else
		{
			mDelayVibrato[L]->SetPhase(0.0);
			mDelayVibrato[R]->SetPhase(0.0);
		}
	}
	else if (parameterID == LifeAudioProcessor::paramAmpOscSyncToggle)
	{
		InvertTremolo = newValue;
		if (InvertTremolo == true)
		{
			mTremolo[L]->SetPhase(0.0);
			mTremolo[R]->SetPhase(float_Pi);
		}
		else
		{
			mTremolo[L]->SetPhase(0.0);
			mTremolo[R]->SetPhase(0.0);
		}
	}
}

//==============================================================================
bool LifeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* LifeAudioProcessor::createEditor()
{
    return new LifeAudioProcessorEditor (*this);
}


float LifeAudioProcessor::RateToFrequency(float rate) const
{
	int iRate = roundFloatToInt(rate);

	switch (iRate)
	{
	case 1: return lastKnownBpm / 120.0;
	case 2: return lastKnownBpm / 60.0;
	case 3: return lastKnownBpm / 30.0;
	case 4: return lastKnownBpm / 15.0;
	default: return lastKnownBpm / 120.0;
	}
}
//==============================================================================
void LifeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	MemoryOutputStream stream(destData, false);
	mState->state.writeToStream(stream);
}

void LifeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	ValueTree tree = ValueTree::readFromData(data, sizeInBytes);
	if (tree.isValid()) {
		mState->state = tree;
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LifeAudioProcessor();
}



