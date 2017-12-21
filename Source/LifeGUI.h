/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

//@AS
#include "TrialDialog.h"
#include "AuthDialog.h"
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
                 public MultiTimer,
                 public CptNotify,
                 public Button::Listener,
                 public Slider::Listener
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

	//Event Timer
	void timerCallback(int timerID) override;

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
    static const char* authorization_png;
    static const int authorization_pngSize;
    static const char* lock2small_png;
    static const int lock2small_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    LifeAudioProcessor& mP;
	//Normalize
	ScopedPointer<NormalisableRange<float>> normalizeDelaySlider;
	ScopedPointer<NormalisableRange<float>> normalizePitchRateSlider;
	ScopedPointer<NormalisableRange<float>> normalizePitchAmountSlider;
	ScopedPointer<NormalisableRange<float>> normalizeFeedbackSlider;
	ScopedPointer<NormalisableRange<float>> normalizeAmplitudeRateSlider;
	ScopedPointer<NormalisableRange<float>> normalizeAmplitudeAmountSlider;
	ScopedPointer<NormalisableRange<float>> normalizeLowPassSlider;
	ScopedPointer<NormalisableRange<float>> normalizeHighPassSlider;
	ScopedPointer<NormalisableRange<float>> normalizeWidthSlider;
	ScopedPointer<NormalisableRange<float>> normalizeWetDrySlider;
	ScopedPointer<NormalisableRange<float>> normalizeGainMasterSlider;
	//Automation
	bool mAutomationDelay;
	bool mAutomationPitchRate;
	bool mAutomationPitchAmount;
	bool mAutomationAmplitudeRate;
	bool mAutomationAmplitudeAmount;
	bool mAutomationFeedback;

	bool mAutomationHighPass;
	bool mAutomationLowPass;

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

   //@AS
   SeqGlob mGlob;
   bool mUnlocked;// will be set true when we are unlocked
   // if set to true then the authorize function will try to reauthorize if
   // expired or within reauth period
   bool mTryReauth;
   // For trial dialog
   SeqTrialDialog mTrialDialog;
   SeqAuthDialog mAuthDlg;
   // Inherited via CptNotify
   virtual void cptValueChange(int cptId, int value) override;
   void authorize();
   void prepareAuthorization(bool allowRenew);
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ToggleButton> pitchOscilationsSyncToggleButton;
    ScopedPointer<ToggleButton> amplitudeOscilationsSyncToggleButton;
    ScopedPointer<Slider> delaySlider;
    ScopedPointer<Slider> pitchRateSlider;
    ScopedPointer<Slider> pitchAmountSlider;
    ScopedPointer<Slider> feedbackSlider;
    ScopedPointer<Slider> stereoWidthSlider;
    ScopedPointer<Slider> amplitudeRateSlider;
    ScopedPointer<Slider> amplitudeAmountSlider;
    ScopedPointer<Slider> highPassFilterSlider;
    ScopedPointer<Slider> loPassFilterSlider;
    ScopedPointer<Slider> wetDrySlider;
    ScopedPointer<Slider> masterGainSlider;
    ScopedPointer<ImageButton> mBtnUnlock;
    Image cachedImage_life_ui_cmbgv3_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LifeGUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
