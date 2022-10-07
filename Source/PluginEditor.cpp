/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//https://www.youtube.com/watch?v=zlCJge8IAfE
//==============================================================================
StutterZenAudioProcessorEditor::StutterZenAudioProcessorEditor (StutterZenAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
reductionKnobL("Reduction", 0, 60, 0.001, 0),
thresHoldKnobL("Threshold", 0, 120, 0.001, 0),
delayKnobL("DelayTime", 0.001, 1, 0.001, 0),
delayWetKnobL("DelayWet", 0.0, 1, 0.01, 0),
outGainKnobL("OutGain", 0, 5, 0.01 , 0),
reductionKnobR("Reduction", 0, 60, 0.001, 0),
thresHoldKnobR("Threshold", 0, 120, 0.001, 0),
delayKnobR("DelayTime", 0.001, 1, 0.001, 0),
delayWetKnobR("DelayWet", 0.0, 1, 0.01, 0),
outGainKnobR("OutGain", 0, 5, 0.01 , 0),

reductionAttachmentL(audioProcessor.treeState, "deltaL", reductionKnobL),
thresholdAttachmentL(audioProcessor.treeState, "thresholdL", thresHoldKnobL),
delayTimeAttachmentL(audioProcessor.treeState, "delayL", delayKnobL),
delayWetAttachmentL(audioProcessor.treeState, "delaywetL", delayWetKnobL),
outGainAttachmentL(audioProcessor.treeState, "gainL", outGainKnobL),
reductionAttachmentR(audioProcessor.treeState, "deltaR", reductionKnobR),
thresholdAttachmentR(audioProcessor.treeState, "thresholdR", thresHoldKnobR),
delayTimeAttachmentR(audioProcessor.treeState, "delayR", delayKnobR),
delayWetAttachmentR(audioProcessor.treeState, "delaywetR", delayWetKnobR),
outGainAttachmentR(audioProcessor.treeState, "gainR", outGainKnobR),
modeAttachment(audioProcessor.treeState,"mode", modeButton)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (650, 800);
    addAndMakeVisible(audioProcessor.audioVisualizer);
    addAndMakeVisible(audioProcessor.audioVisualizer2);
    audioProcessor.audioVisualizer.setColours(juce::Colours::black, juce::Colours::whitesmoke);
    audioProcessor.audioVisualizer2.setColours(juce::Colours::black, juce::Colours::whitesmoke);
    reductionKnobL.setStyles(juce::Zen_Knob::ZenStyles::redknob);
    reductionKnobL.setTextValueSuffix(" ReductionL");
    addAndMakeVisible(&reductionKnobL);
    
    thresHoldKnobL.setStyles(juce::Zen_Knob::ZenStyles::redknob);
    thresHoldKnobL.setTextValueSuffix(" ThresholdL");
    addAndMakeVisible(&thresHoldKnobL);

    
    delayKnobL.setStyles(juce::Zen_Knob::ZenStyles::redknob);
    delayKnobL.setTextValueSuffix(" DelayTimeL");
    addAndMakeVisible(&delayKnobL);
    
    
    delayWetKnobL.setStyles(juce::Zen_Knob::ZenStyles::redknob);
    delayWetKnobL.setTextValueSuffix(" DelayWetL");
    addAndMakeVisible(&delayWetKnobL);
    
    outGainKnobL.setStyles(juce::Zen_Knob::ZenStyles::redknob);
    outGainKnobL.setTextValueSuffix(" OutGainL");
    addAndMakeVisible(&outGainKnobL);

    reductionKnobR.setStyles(juce::Zen_Knob::ZenStyles::greenknob);
    reductionKnobR.setTextValueSuffix(" ReductionR");
    addAndMakeVisible(&reductionKnobR);
    
    thresHoldKnobR.setStyles(juce::Zen_Knob::ZenStyles::greenknob);
    thresHoldKnobR.setTextValueSuffix(" ThresholdR");
    addAndMakeVisible(&thresHoldKnobR);

   
    
    delayKnobR.setStyles(juce::Zen_Knob::ZenStyles::greenknob);
    delayKnobR.setTextValueSuffix(" DelayTimeR");
    addAndMakeVisible(&delayKnobR);
    
    
    delayWetKnobR.setStyles(juce::Zen_Knob::ZenStyles::greenknob);
    delayWetKnobR.setTextValueSuffix(" DelayWetR");
    addAndMakeVisible(&delayWetKnobR);
    
    outGainKnobR.setStyles(juce::Zen_Knob::ZenStyles::greenknob);
    outGainKnobR.setTextValueSuffix(" OutGainR");
    addAndMakeVisible(&outGainKnobR);


    modeButton.setButtonText("Link");
    modeButton.setToggleable(true);
    modeButton.setClickingTogglesState(true);
   
    modeButton.addListener(this);
    modeButton.setEnabled(true);
    addAndMakeVisible(&modeButton);

   
    //not the best solution but it works :)
    modeButton.triggerClick();
    modeButton.triggerClick();
    
}

StutterZenAudioProcessorEditor::~StutterZenAudioProcessorEditor()
{
   modeButton.removeListener(this);
}

//==============================================================================
void StutterZenAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
   // modeButton.setToggleState(audioProcessor.treeState.getRawParameterValue("mode"), juce::dontSendNotification);
    g.fillAll(juce::Colours::black);
}

void StutterZenAudioProcessorEditor::resized()
{
  
    audioProcessor.audioVisualizer.setBounds(100,100,300,100);
    audioProcessor.audioVisualizer2.setBounds(100,600,300,100);
    thresHoldKnobL.setBounds(50,300,100,100);
    reductionKnobL.setBounds(150,300,100,100);
    delayKnobL.setBounds(250,300,100,100);
    delayWetKnobL.setBounds(350,300,100,100);
    outGainKnobL.setBounds(450,300,100,100);
    thresHoldKnobR.setBounds(50,450,100,100);
    reductionKnobR.setBounds(150,450,100,100);
    delayKnobR.setBounds(250,450,100,100);
    delayWetKnobR.setBounds(350,450,100,100);
    outGainKnobR.setBounds(450,450,100,100);
    modeButton.setBounds(550,250,100,100);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void StutterZenAudioProcessorEditor::buttonStateChanged(juce::Button* button)
{
   
    if (button == &modeButton)
    {
        if (modeButton.getToggleState() == false)
        {
            thresHoldKnobL.onValueChange = [this]()
            {
                audioProcessor.audioVisualizer.updateLineL(thresHoldKnobL.getValue());
            };
            
            thresHoldKnobR.onValueChange = [this]()
            {
                audioProcessor.audioVisualizer.updateLineR(thresHoldKnobR.getValue());
            };
            
            reductionKnobL.onValueChange = [this](){};
            reductionKnobR.onValueChange = [this](){};
            delayKnobL.onValueChange = [this](){};
            delayKnobR.onValueChange = [this](){};
            delayWetKnobL.onValueChange = [this](){};
            delayWetKnobR.onValueChange = [this](){};
            outGainKnobL.onValueChange = [this](){};
            outGainKnobR.onValueChange = [this](){};
        } else if (modeButton.getToggleState() == true)
        {
            thresHoldKnobL.onValueChange = [this]()
            {
                thresHoldKnobR.setValue(thresHoldKnobL.getValue());
                audioProcessor.audioVisualizer.updateLineL(thresHoldKnobL.getValue());
                audioProcessor.audioVisualizer.updateLineR(thresHoldKnobR.getValue());
            };
            
            thresHoldKnobR.onValueChange = [this]()
            {
                thresHoldKnobL.setValue(thresHoldKnobR.getValue());
                audioProcessor.audioVisualizer.updateLineL(thresHoldKnobL.getValue());
                audioProcessor.audioVisualizer.updateLineR(thresHoldKnobR.getValue());
            };
            reductionKnobL.onValueChange = [this]()
            {
                reductionKnobR.setValue(reductionKnobL.getValue());
            };
            reductionKnobR.onValueChange = [this]()
            {
                reductionKnobL.setValue(reductionKnobR.getValue());
            };
            delayKnobL.onValueChange = [this]()
            {
                delayKnobR.setValue(delayKnobL.getValue());
            };
            delayKnobR.onValueChange = [this]()
            {
                delayKnobL.setValue(delayKnobR.getValue());
            };
            delayWetKnobL.onValueChange = [this]()
            {
                delayWetKnobR.setValue(delayWetKnobL.getValue());
            };
            delayWetKnobR.onValueChange = [this]()
            {
                delayWetKnobL.setValue(delayWetKnobR.getValue());
            };
            
            outGainKnobL.onValueChange = [this]()
            {
                outGainKnobR.setValue(outGainKnobL.getValue());
            };
            outGainKnobR.onValueChange = [this]()
            {
                outGainKnobL.setValue(outGainKnobR.getValue());
            };
        }
        
    }
}

