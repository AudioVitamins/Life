/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
LifeAudioProcessorEditor::LifeAudioProcessorEditor (LifeAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

LifeAudioProcessorEditor::~LifeAudioProcessorEditor()
{
}

//==============================================================================
void LifeAudioProcessorEditor::paint (Graphics& g)
{

}

void LifeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
