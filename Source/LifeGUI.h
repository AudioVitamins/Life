/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_1A035C6AA1EC25D6__
#define __JUCE_HEADER_1A035C6AA1EC25D6__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
class LifeAudioProcessor;
class SliderComponent;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LifeGUI  : public Component,
                 public Timer,
                 public ButtonListener,
                 public SliderListener
{
public:
    //==============================================================================
    LifeGUI (LifeAudioProcessor& p);
    ~LifeGUI();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	struct KnobImageInfo {
		Image &brgImg;
		int numOfImages;
		float FirstKnobX;
		float FirstKnobY;
		float WidthKnob;
		float HightKnob;
		float SpaceEachKnob;
		KnobImageInfo(Image &img,
			int numOfImages = 101,// Number of Know image stage
			float firstKnobX = 9.0f,
			float firstKnobY = 9.0f,
			float widthKnob = 42.0f,
			float hightKnob = 42.0f,
			float spaceEachKnob = 18.0f) :
			brgImg(img),
			numOfImages(numOfImages),
			FirstKnobX(firstKnobX),
			FirstKnobY(firstKnobY),
			WidthKnob(widthKnob),
			HightKnob(hightKnob),
			SpaceEachKnob(spaceEachKnob)
		{
		}
	};

	class CustomSlider : public LookAndFeel_V3
	{
	public:
		class SliderLabelComp : public Label
		{
		public:
			SliderLabelComp() : Label(String::empty, String::empty) {}

			void mouseWheelMove(const MouseEvent&, const MouseWheelDetails&) override {}
			void mouseDoubleClick(const MouseEvent&) override {
				this->showEditor();
			};
		};
	private:
		KnobImageInfo &knob;
	public:
		CustomSlider(KnobImageInfo &knobInfo) : LookAndFeel_V3(), knob(knobInfo) {
			LookAndFeel::setDefaultLookAndFeel(this);
		};

		~CustomSlider() {};
		Label* createSliderTextBox(Slider& slider) override;
		void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
			const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;

	};

	class LifeToggleButton : public LookAndFeel_V3
	{
	public:
		
		~LifeToggleButton() {};

		void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
			bool isMouseOverButton, bool isButtonDown) override;

		void drawToggleButton(Graphics& g, ToggleButton& button, bool isMouseOverButton,
			bool isButtonDown) override;

		Image CachedImage_LifeToggleButtonVertical_png;
			
	};

	class LifeLinkButton : public LookAndFeel_V3
	{
	public:

		~LifeLinkButton() {};

		void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
			bool isMouseOverButton, bool isButtonDown) override;

		void drawToggleButton(Graphics& g, ToggleButton& button, bool isMouseOverButton,
			bool isButtonDown) override;

		Image CachedImage_LifeLinkButtonVertical_png;

	};

	//Event Timer
	void timerCallback() override;

	//Slider Handler
	void sliderDragStarted(Slider* sliderThatWasMoved) override;
	void sliderDragEnded(Slider* sliderThatWasMoved) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

    // Binary resources:
    static const char* life_ui_cmversion_png;
    static const int life_ui_cmversion_pngSize;
    static const char* life_ui_cmversionbg_png;
    static const int life_ui_cmversionbg_pngSize;
    static const char* whitered_dial_101increments_vertical_60x60_png;
    static const int whitered_dial_101increments_vertical_60x60_pngSize;
    static const char* whitered_rotaryswitch_6pos_vertical_60x60_png;
    static const int whitered_rotaryswitch_6pos_vertical_60x60_pngSize;
    static const char* whitered_rotaryswitch_4pos_vertical_60x60_png;
    static const int whitered_rotaryswitch_4pos_vertical_60x60_pngSize;
    static const char* life_ui_cmversionbgv2_png;
    static const int life_ui_cmversionbgv2_pngSize;
    static const char* life_ui_cmbgv3_png;
    static const int life_ui_cmbgv3_pngSize;
	static const char* life_ui_bg_png;
	static const int life_ui_bg_pngSize;
	static const char* white_slideswitch_2pos_vertical_50x50_png;
	static const int white_slideswitch_2pos_vertical_50x50_pngSize;
	static const char* white_pushbutton_redlink_25x25_vertical_png;
	static const int white_pushbutton_redlink_25x25_vertical_pngSize;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    LifeAudioProcessor& mP;

	//Normalize
	ScopedPointer<NormalisableRange<float>> normalizeDelaySlider[2];
	ScopedPointer<NormalisableRange<float>> normalizePitchRateSlider[2];
	ScopedPointer<NormalisableRange<float>> normalizePitchAmountSlider[2];
	ScopedPointer<NormalisableRange<float>> normalizeFeedbackSlider[2];
	ScopedPointer<NormalisableRange<float>> normalizeAmplitudeRateSlider[2];
	ScopedPointer<NormalisableRange<float>> normalizeAmplitudeAmountSlider[2];
	ScopedPointer<NormalisableRange<float>> normalizeLowPassSlider[2];
	ScopedPointer<NormalisableRange<float>> normalizeHighPassSlider[2];
	ScopedPointer<NormalisableRange<float>> normalizeWidthSlider;
	ScopedPointer<NormalisableRange<float>> normalizeWetDrySlider;
	ScopedPointer<NormalisableRange<float>> normalizeGainMasterSlider;

	bool DelayLink = false;
	bool FeedbackLink = false;

	//Automation
	bool mAutomationDelay[2];
	bool mAutomationPitchRate[2];
	bool mAutomationPitchAmount[2];
	bool mAutomationAmplitudeRate[2];
	bool mAutomationAmplitudeAmount[2];
	bool mAutomationFeedback[2];

	bool mAutomationHighPass[2];
	bool mAutomationLowPass[2];

	bool mAutomationWidth;
	bool mAutomationWetDry;

	bool mAutomationGainMaster;

	// Background
	Image bgrImgDelay;
	ScopedPointer<KnobImageInfo> knobInfoDelay;
	ScopedPointer<CustomSlider> knobLookDelay;

	Image bgrImgRate;
	ScopedPointer<KnobImageInfo> knobInfoRate;
	ScopedPointer<CustomSlider> knobLookRate;

	Image bgrImgAmount;
	ScopedPointer<KnobImageInfo> knobInfoAmount;
	ScopedPointer<CustomSlider> knobLookAmount;
	
	ScopedPointer<LifeToggleButton> LifeToggleButtonLookandFeel;
	ScopedPointer<LifeLinkButton> LifeLinkButtonLookandFeel;

	Image CachedImage_Life_UI_Background_v1_png;
	
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ToggleButton> pitchOscilationsSyncToggleButton;
    ScopedPointer<ToggleButton> amplitudeOscilationsSyncToggleButton;
	ScopedPointer<ToggleButton> LR_Or_MS_ToggleButton;
	ScopedPointer<ToggleButton> LinkDelayToggleButton;
	ScopedPointer<ToggleButton> LinkFeedbackToggleButton;
    ScopedPointer<Slider> delaySlider[2];
    ScopedPointer<Slider> pitchRateSlider[2];
    ScopedPointer<Slider> pitchAmountSlider[2];
    ScopedPointer<Slider> feedbackSlider[2];
    ScopedPointer<Slider> stereoWidthSlider;
    ScopedPointer<Slider> amplitudeRateSlider[2];
    ScopedPointer<Slider> amplitudeAmountSlider[2];
    ScopedPointer<Slider> highPassFilterSlider[2];
    ScopedPointer<Slider> loPassFilterSlider[2];
    ScopedPointer<Slider> wetDrySlider;
    ScopedPointer<Slider> masterGainSlider;
    Image cachedImage_life_ui_cmbgv3_png_1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LifeGUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_1A035C6AA1EC25D6__
