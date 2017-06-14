/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GUI  : public Component,
             public ButtonListener,
             public SliderListener
{
public:
    //==============================================================================
    GUI ();
    ~GUI();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

    // Binary resources:
    static const char* whitered_dial_101increments_vertical_40x40_png;
    static const int whitered_dial_101increments_vertical_40x40_pngSize;
    static const char* whitered_dial_51increments_vertical_40x40_png;
    static const int whitered_dial_51increments_vertical_40x40_pngSize;
    static const char* white_slideswitch_2pos_horizontal__33x33_png;
    static const int white_slideswitch_2pos_horizontal__33x33_pngSize;
    static const char* whitered_rotaryswitch_6pos_40x40_png;
    static const int whitered_rotaryswitch_6pos_40x40_pngSize;
    static const char* whitered_rotaryswitch_4pos_40x40_png;
    static const int whitered_rotaryswitch_4pos_40x40_pngSize;
    static const char* life_ui_cmversion_png;
    static const int life_ui_cmversion_pngSize;
    static const char* life_ui_cmversionbg_png;
    static const int life_ui_cmversionbg_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ToggleButton> toggleButton;
    ScopedPointer<ToggleButton> toggleButton2;
    ScopedPointer<Slider> slider;
    ScopedPointer<Slider> slider2;
    ScopedPointer<Slider> slider3;
    ScopedPointer<Slider> slider4;
    ScopedPointer<Slider> slider5;
    ScopedPointer<Slider> slider6;
    ScopedPointer<Slider> slider7;
    ScopedPointer<Slider> slider8;
    ScopedPointer<Slider> slider9;
    ScopedPointer<Slider> slider10;
    ScopedPointer<Slider> slider11;
    Image cachedImage_life_ui_cmversionbg_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
