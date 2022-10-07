/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Zen_Knob.h"
//==============================================================================
/**
*/
class StutterZenAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::ToggleButton::Listener
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
    juce::Zen_Knob reductionKnobR,thresHoldKnobR, delayKnobR, delayWetKnobR, outGainKnobR;
    
    using APVTS = juce::AudioProcessorValueTreeState;
                using Attachment = APVTS::SliderAttachment;
            Attachment reductionAttachmentL, thresholdAttachmentL, delayTimeAttachmentL, delayWetAttachmentL,outGainAttachmentL;
            Attachment reductionAttachmentR, thresholdAttachmentR, delayTimeAttachmentR, delayWetAttachmentR,outGainAttachmentR;
    juce::ToggleButton modeButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment modeAttachment;
    
    void buttonClicked (juce::Button* button) override  {};
    void buttonStateChanged (juce::Button* button) override ;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StutterZenAudioProcessorEditor)
};
