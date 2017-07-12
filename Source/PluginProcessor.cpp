/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LogUtil.h"
String LifeAudioProcessor::paramDelay = "Delay";
String LifeAudioProcessor::paramPitchRate = "PitchRate";
String LifeAudioProcessor::paramPitchAmount = "PitchAmount";
String LifeAudioProcessor::paramFeedback = "Feedback";

String LifeAudioProcessor::paramAmplitudeRate = "AmplitudeRate";
String LifeAudioProcessor::paramAmplitudeAmount = "AmplitudeAmount";

String LifeAudioProcessor::paramHighFreq = "HighFreq";
String LifeAudioProcessor::paramLowFreq = "LowFreq";

String LifeAudioProcessor::paramWidth = "Width";
String LifeAudioProcessor::paramWetDry = "WetDry";

String LifeAudioProcessor::paramGainMaster = "GainMaster";
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
	mState->createAndAddParameter(paramDelay, "Delay", TRANS("Delay"), NormalisableRange<float>(0.0f, 100.0f, 0.01), 0.0, nullptr, nullptr);
	mState->createAndAddParameter(paramPitchRate, "Pitch Rate", TRANS("Pitch Rate"), NormalisableRange<float>(1.0f, 4.0f, 1), 1.0, nullptr, nullptr);
	mState->createAndAddParameter(paramPitchAmount, "Pitch Amount", TRANS("Pitch Amount"), NormalisableRange<float>(0.0f, 5.0f, 1), 0.0, nullptr, nullptr);
	mState->createAndAddParameter(paramFeedback, "Feedback", TRANS("Feedback"), NormalisableRange<float>(0.0f, 100.0f, 0.1), 50.0, nullptr, nullptr);

	mState->createAndAddParameter(paramAmplitudeRate, "Amplitude Rate", TRANS("Amplitude Rate"), NormalisableRange<float>(1.0f, 4.0f, 1), 1.0, nullptr, nullptr);
	mState->createAndAddParameter(paramAmplitudeAmount, "Amplitude Amount", TRANS("Amplitude Amount"), NormalisableRange<float>(0.0f, 5.0f, 1), 0.0, nullptr, nullptr);

	mState->createAndAddParameter(paramHighFreq, "High Freq", TRANS("High Freq"), NormalisableRange<float>(20.0f, 20000.0f, 0.1), 20.0, nullptr, nullptr);
	mState->createAndAddParameter(paramLowFreq, "Low Freq", TRANS("Low Freq"), NormalisableRange<float>(20.0f, 20000.0f, 0.1), 20000.0, nullptr, nullptr);

	mState->createAndAddParameter(paramWidth, "Width", TRANS("Width"), NormalisableRange<float>(0.0f, 2.0f, 0.01), 1.0, nullptr, nullptr);
	mState->createAndAddParameter(paramWetDry, "Wet Dry", TRANS("Wet Dry"), NormalisableRange<float>(0.0f, 100.0f, 0.1), 50.0, nullptr, nullptr);

	mState->createAndAddParameter(paramGainMaster, "Gain Master", TRANS("Gain Master"), NormalisableRange<float>(-10.0f, 10.0f, 0.1), 0.0, nullptr, nullptr);
	mState->state = ValueTree("LifeParameters");

	mState->addParameterListener(paramDelay, this);
	mState->addParameterListener(paramPitchRate, this);
	mState->addParameterListener(paramPitchAmount, this);
	mState->addParameterListener(paramFeedback, this);

	mState->addParameterListener(paramAmplitudeRate, this);
	mState->addParameterListener(paramAmplitudeAmount, this);

	mState->addParameterListener(paramHighFreq, this);
	mState->addParameterListener(paramLowFreq, this);

	mState->addParameterListener(paramWidth, this);
	mState->addParameterListener(paramWetDry, this);

	mState->addParameterListener(paramGainMaster, this);
    float sampleRate = 44100.0;
    int numOutputChannel = 2;
    mDelayVibrato = new Jimmy::DSP::DelayVibrato(float(sampleRate), 0.1f, numOutputChannel);
    mTremolo = new Jimmy::DSP::Tremolo(float(sampleRate), numOutputChannel);
    
    mFilterHP = new Jimmy::DSP::IIRFilterHP(float(sampleRate), numOutputChannel);
    mFilterLP = new Jimmy::DSP::IIRFilterLP(float(sampleRate), numOutputChannel);
    
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	int numOutputChannel = getTotalNumOutputChannels();
    
	mDelayVibrato = new Jimmy::DSP::DelayVibrato(float(sampleRate), 0.1f, numOutputChannel);
	mTremolo = new Jimmy::DSP::Tremolo(float(sampleRate), numOutputChannel);

	mFilterHP = new Jimmy::DSP::IIRFilterHP(float(sampleRate), numOutputChannel);
	mFilterLP = new Jimmy::DSP::IIRFilterLP(float(sampleRate), numOutputChannel);

	mWidth = new Jimmy::DSP::Width();
	mWet = new Jimmy::DSP::WetDry();
    
    mGainMaster = new Jimmy::DSP::GainMaster(-10.0f, 10.0f, numOutputChannel);
    
	//mDelay = new Jimmy::DSP::StaticDelay(float(sampleRate), 0.1f, numOutputChannel);
	// Aplly Pitch/Feedback
	mDelayVibrato->preparePlay(sampleRate, samplesPerBlock);
	float *delayMs = mState->getRawParameterValue(paramDelay);
	mDelayVibrato->setDelayInMiliSec(*delayMs);
	//mDelay->preparePlay(samplesPerBlock);
	//mDelay->setDelayInMiliSec(*delayMs);

	float *ratePitch = mState->getRawParameterValue(paramPitchRate);
	float *amountPitch = mState->getRawParameterValue(paramPitchAmount);
	float *feedback = mState->getRawParameterValue(paramFeedback);
	float freqPitch = RateToFrequency(*ratePitch);
	//mVibrato->preparePlay(sampleRate);
//	mVibrato->SetFrequency(freqPitch);
//	mVibrato->SetDepth(*amountPitch);
//	mVibrato->SetFeedback(*feedback);
	mDelayVibrato->SetFrequency(freqPitch);
	mDelayVibrato->SetDepth(*amountPitch);
	mDelayVibrato->SetFeedback(*feedback);
	
	// Apply AM
	float *rateAm = mState->getRawParameterValue(paramAmplitudeRate);
	float freqAm = RateToFrequency(*rateAm);
	float *amountAm = mState->getRawParameterValue(paramAmplitudeAmount);
	mTremolo->SetFrequency(freqAm);
	mTremolo->SetDepth(*amountAm);

	// Apply Filter
	float *highFreq = mState->getRawParameterValue(paramHighFreq);
	mFilterHP->changeCutOff(*highFreq);

	float *lowFreq = mState->getRawParameterValue(paramLowFreq);
	mFilterLP->changeCutOff(*lowFreq);
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
	const int totalNumInputChannels = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();
	// Clear/Reset apart of output chans
	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

    bool ok = getPlayHead()->getCurrentPosition(info);
    float *ratePitch = mState->getRawParameterValue(paramPitchRate);
    float freq = RateToFrequency(*ratePitch);
    mDelayVibrato->SetFrequency(freq);
	AudioSampleBuffer raw;
	raw.makeCopyOf(buffer);
	///
	mDelayVibrato->process(buffer);
	//Delay
 	//mDelay->process(buffer);

	// Aplly Pitch/Feedback
	//mVibrato->process(buffer);

	// Apply AM
	mTremolo->process(buffer);

	// Apply Filter
	mFilterHP->process(buffer);
	
	mFilterLP->process(buffer);
	
	//Apply Width
	mWidth->process(buffer);

	//Apply Dry/Wet
	mWet->process(raw, buffer);

	// Apply gain
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

	if (parameterID == LifeAudioProcessor::paramDelay) {
//		suspendProcessing(true);
		//mDelay->setDelayInMiliSec(newValue);
		mDelayVibrato->setDelayInMiliSec(newValue);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramPitchRate) {
//		suspendProcessing(true);
		//float *ratePitch = mState->getRawParameterValue(paramPitchRate);
		float freqPitch = RateToFrequency(newValue);
		mDelayVibrato->SetFrequency(freqPitch);
		//mVibrato->SetFrequency(freqPitch);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramPitchAmount) {
//		suspendProcessing(true);
		//float *amountPitch = mState->getRawParameterValue(paramPitchAmount);
		//mVibrato->SetDepth(newValue);
		mDelayVibrato->SetDepth(newValue);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramFeedback) {
//		suspendProcessing(true);
		//float *feedback = mState->getRawParameterValue(paramFeedback);
		//mVibrato->SetFeedback(newValue);
		mDelayVibrato->SetFeedback(newValue);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramAmplitudeRate) {
//		suspendProcessing(true);
		//float *rateAm = mState->getRawParameterValue(paramAmplitudeRate);
		float freqAm = RateToFrequency(newValue);
		mTremolo->SetFrequency(freqAm);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramAmplitudeAmount) {
//		suspendProcessing(true);
		//float *amountAm = mState->getRawParameterValue(paramAmplitudeAmount);
		mTremolo->SetDepth(newValue);
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramHighFreq) {
		// Apply Filter
//		suspendProcessing(true);
		mFilterHP->changeCutOff(newValue);
//		mFilterHP->reset();
//		suspendProcessing(false);
	}
	else if (parameterID == LifeAudioProcessor::paramLowFreq) {
		// Apply Filter
//		suspendProcessing(true);
		mFilterLP->changeCutOff(newValue);
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


float LifeAudioProcessor::RateToFrequency(float rate) {
	int iRate = roundFloatToInt(rate);
	AudioPlayHead::CurrentPositionInfo info;
	bool ok = getPlayHead()->getCurrentPosition(info);
	switch (iRate)
	{
	case 1:
		return info.bpm / 120;
		break;
	case 2:
		return info.bpm / 60;
		break;
	case 3:
		return info.bpm / 30;
		break;
	case 4:
		return info.bpm / 15;
		break;
	default:
		return info.bpm / 120;
		break;
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



