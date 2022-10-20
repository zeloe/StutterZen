/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Zen_Knob.h"
#include "GuiDefines.h"
#include "ValueTreeC.h"
//==============================================================================
/**
*/
class StutterZenAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::ToggleButton::Listener,public juce::Timer
{
public:
    StutterZenAudioProcessorEditor (StutterZenAudioProcessor&);
    ~StutterZenAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StutterZenAudioProcessor& audioProcessor;
    juce::Zen_Knob reductionKnobL,thresHoldKnobL, delayKnobL, delayWetKnobL, outGainKnobL;
    juce::Zen_Knob reductionKnobR,thresHoldKnobR, outGainKnobR;
    juce::Zen_Knob speedDelayKnob, speedChorusKnob;
    juce::Zen_Knob wetDelayKnob, wetChorusKnob;
    using APVTS = juce::AudioProcessorValueTreeState;
                using Attachment = APVTS::SliderAttachment;
            Attachment reductionAttachmentL, thresholdAttachmentL, outGainAttachmentL;
    Attachment reductionAttachmentR, thresholdAttachmentR, outGainAttachmentR,speedDelayAttachment, speedChorusAttachment,wetDelayAttachment, wetChorusAttachment;
    juce::ToggleButton modeButton;
    juce::TextButton speedbutton1, speedbutton2, stopbutton1, stopbutton2, resetButton;
    
    juce::AudioProcessorValueTreeState::ButtonAttachment modeAttachment;
    juce::Colour colour1 = juce::Colours::white;
    void buttonClicked (juce::Button* button) override  {};
    void buttonStateChanged (juce::Button* button) override ;
    void timerCallback() override;
    int count = 0;
    int x1 = 295;
    int y1 = 300;
    bool flag1 = false;
    ValueTreeC valueTreeC1, valueTreeC2, valueTreeC3, valueTreeC4;
    DelayView delayView1, delayView2, delayView3, delayView4;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StutterZenAudioProcessorEditor)
};
