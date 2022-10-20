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
outGainKnobR("OutGain", 0, 5, 0.01 , 0),
speedDelayKnob("SpeedDelay", 0, 5, 0.01 , 0),
speedChorusKnob("SpeedChours", 0, 5, 0.01 , 0),
wetDelayKnob("WetDelay", 0, 5, 0.01 , 0),
wetChorusKnob("WetChours", 0, 5, 0.01 , 0),
reductionAttachmentL(audioProcessor.treeState, "deltaL", reductionKnobL),
thresholdAttachmentL(audioProcessor.treeState, "thresholdL", thresHoldKnobL),
outGainAttachmentL(audioProcessor.treeState, "gainL", outGainKnobL),
reductionAttachmentR(audioProcessor.treeState, "deltaR", reductionKnobR),
thresholdAttachmentR(audioProcessor.treeState, "thresholdR", thresHoldKnobR),
outGainAttachmentR(audioProcessor.treeState, "gainR", outGainKnobR),
speedDelayAttachment(audioProcessor.treeState,"speedDelay", speedDelayKnob),
speedChorusAttachment(audioProcessor.treeState,"speedChorus", speedChorusKnob),
wetDelayAttachment(audioProcessor.treeState,"wetDelay", wetDelayKnob),
wetChorusAttachment(audioProcessor.treeState,"wetChorus", wetChorusKnob),
modeAttachment(audioProcessor.treeState,"mode", modeButton),
valueTreeC1(audioProcessor.treeState,delayView1,"positions1"),
valueTreeC2(audioProcessor.treeState,delayView2,"positions2"),
valueTreeC3(audioProcessor.treeState,delayView3,"positions3"),
valueTreeC4(audioProcessor.treeState,delayView4,"positions4")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (650, 1000);
    startTimerHz(33);
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
 
    addAndMakeVisible(&delayView1);
    addAndMakeVisible(&delayView2);
    addAndMakeVisible(&delayView3);
    addAndMakeVisible(&delayView4);
 
    speedbutton1.setButtonText("DelaySpeed");
    speedbutton1.addListener(this);
    speedbutton1.onClick = [this]()
    {
        delayView1.point1.changeSpeed();
        delayView1.point2.changeSpeed();
        delayView1.point3.changeSpeed();
        delayView1.point4.changeSpeed();
        delayView2.point1.changeSpeed();
        delayView2.point2.changeSpeed();
        delayView2.point3.changeSpeed();
        delayView2.point4.changeSpeed();
        valueTreeC1.valueTreePropertyChanged();
        valueTreeC2.valueTreePropertyChanged();
    };
    addAndMakeVisible(&speedbutton1);
    speedbutton2.addListener(this);
    speedbutton2.onClick = [this]()
    {
        delayView3.point1.changeSpeed();
        delayView3.point2.changeSpeed();
        delayView3.point3.changeSpeed();
        delayView3.point4.changeSpeed();
        delayView4.point1.changeSpeed();
        delayView4.point2.changeSpeed();
        delayView4.point3.changeSpeed();
        delayView4.point4.changeSpeed();
        valueTreeC3.valueTreePropertyChanged();
        valueTreeC4.valueTreePropertyChanged();
    };
    speedbutton2.setButtonText("ChorusSpeed");
    addAndMakeVisible(&speedbutton2);
    stopbutton1.setButtonText("DelayStop");
    stopbutton1.addListener(this);
    stopbutton1.onClick = [this]()
    {
        delayView1.point1.reset();
        delayView1.point2.reset();
        delayView1.point3.reset();
        delayView1.point4.reset();
        delayView2.point1.reset();
        delayView2.point2.reset();
        delayView2.point3.reset();
        delayView2.point4.reset();
        valueTreeC1.valueTreePropertyChanged();
        valueTreeC2.valueTreePropertyChanged();
    };
    
    addAndMakeVisible(&stopbutton1);
    
    stopbutton2.setButtonText("ChorusStop");
    stopbutton2.addListener(this);
    stopbutton2.onClick = [this]()
    {
       
        delayView3.point1.reset();
        delayView3.point2.reset();
        delayView3.point3.reset();
        delayView3.point4.reset();
        delayView4.point1.reset();
        delayView4.point2.reset();
        delayView4.point3.reset();
        delayView4.point4.reset();
        valueTreeC3.valueTreePropertyChanged();
        valueTreeC4.valueTreePropertyChanged();
    };
    addAndMakeVisible(&stopbutton2);
    
   
    speedDelayKnob.setStyles(juce::Zen_Knob::ZenStyles::blueknob);
    speedDelayKnob.setTextValueSuffix(" ScaleSpeedDelay");
    addAndMakeVisible(&speedDelayKnob);
    speedDelayKnob.onValueChange = [this]()
    {
        
        delayView1.point1.scaleSpeed(speedDelayKnob.getValue());
        delayView1.point2.scaleSpeed(speedDelayKnob.getValue());
        delayView1.point3.scaleSpeed(speedDelayKnob.getValue());
        delayView1.point4.scaleSpeed(speedDelayKnob.getValue());
        delayView2.point1.scaleSpeed(speedDelayKnob.getValue());
        delayView2.point2.scaleSpeed(speedDelayKnob.getValue());
        delayView2.point3.scaleSpeed(speedDelayKnob.getValue());
        delayView2.point4.scaleSpeed(speedDelayKnob.getValue());
        valueTreeC1.valueTreePropertyChanged();
        valueTreeC2.valueTreePropertyChanged();
       
    };
    
    
    
    speedChorusKnob.setStyles(juce::Zen_Knob::ZenStyles::blueknob);
    speedChorusKnob.setTextValueSuffix(" ScaleSpeedChorus");
    addAndMakeVisible(&speedChorusKnob);
    speedChorusKnob.onValueChange = [this]()
    {
        delayView3.point1.scaleSpeed(speedChorusKnob.getValue());
        delayView3.point2.scaleSpeed(speedChorusKnob.getValue());
        delayView3.point3.scaleSpeed(speedChorusKnob.getValue());
        delayView3.point4.scaleSpeed(speedChorusKnob.getValue());
        delayView4.point1.scaleSpeed(speedChorusKnob.getValue());
        delayView4.point2.scaleSpeed(speedChorusKnob.getValue());
        delayView4.point3.scaleSpeed(speedChorusKnob.getValue());
        delayView4.point4.scaleSpeed(speedChorusKnob.getValue());
        valueTreeC3.valueTreePropertyChanged();
        valueTreeC4.valueTreePropertyChanged();
    };
    
    wetDelayKnob.setStyles(juce::Zen_Knob::ZenStyles::lightblueknob);
    wetDelayKnob.setTextValueSuffix(" DelayMix");
    addAndMakeVisible(&wetDelayKnob);
    
    
    wetChorusKnob.setStyles(juce::Zen_Knob::ZenStyles::lightblueknob);
    wetChorusKnob.setTextValueSuffix(" ChorusMix");
    addAndMakeVisible(&wetChorusKnob);
    resetButton.setButtonText("Reset");
    resetButton.addListener(this);
    resetButton.onClick = [this]()
    {
        delayView1.point1.resetPos();
        delayView1.point2.resetPos();
        delayView1.point3.resetPos();
        delayView1.point4.resetPos();
        delayView2.point1.resetPos();
        delayView2.point2.resetPos();
        delayView2.point3.resetPos();
        delayView2.point4.resetPos();
        delayView3.point1.resetPos();
        delayView3.point2.resetPos();
        delayView3.point3.resetPos();
        delayView3.point4.resetPos();
        delayView4.point1.resetPos();
        delayView4.point2.resetPos();
        delayView4.point3.resetPos();
        delayView4.point4.resetPos();
        speedbutton1.triggerClick();
        speedbutton2.triggerClick();
        valueTreeC1.valueTreePropertyChanged();
        valueTreeC2.valueTreePropertyChanged();
        valueTreeC3.valueTreePropertyChanged();
        valueTreeC4.valueTreePropertyChanged();

    };
    addAndMakeVisible(&resetButton);
}

StutterZenAudioProcessorEditor::~StutterZenAudioProcessorEditor()
{
   modeButton.removeListener(this);
    speedbutton1.removeListener(this);
    speedbutton2.removeListener(this);
    stopbutton1.removeListener(this);
    stopbutton2.removeListener(this);
    resetButton.removeListener(this);
}

//==============================================================================
void StutterZenAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
   // modeButton.setToggleState(audioProcessor.treeState.getRawParameterValue("mode"), juce::dontSendNotification);
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setColour(colour1);
    audioProcessor.audioEngine.updateDelayL1(juce::jmap(delayView1.point1.getPosX() ,0.f,float(width1),0.001f,.002f));
    audioProcessor.audioEngine.updateDelayL2((juce::jmap(delayView1.point2.getPosX()  ,0.f,float(width1),0.001f,.002f)));
    audioProcessor.audioEngine.updateDelayL3((juce::jmap(delayView1.point3.getPosX() ,0.f,float(width1),0.001f,.002f)));
    audioProcessor.audioEngine.updateDelayL4((juce::jmap(delayView1.point4.getPosX() ,0.f,float(width1),0.001f,.002f)));
    audioProcessor.audioEngine.updateDelayR1((juce::jmap(delayView2.point1.getPosX() ,0.f,float(width1),0.001f,.002f)));
    audioProcessor.audioEngine.updateDelayR2((juce::jmap(delayView2.point2.getPosX() ,0.f,float(width1),0.001f,.002f)));
    audioProcessor.audioEngine.updateDelayR3((juce::jmap(delayView2.point3.getPosX() ,0.f,float(width1),0.001f,.002f)));
    audioProcessor.audioEngine.updateDelayR4((juce::jmap(delayView2.point4.getPosX()  ,0.f,float(width1),0.001f,.002f)));
    audioProcessor.audioEngine.updateDelayWetL1((juce::jmap(delayView1.point1.getPosY()  ,0.f,float(height1),0.f,.7f)));
    audioProcessor.audioEngine.updateDelayWetL2((juce::jmap(delayView1.point2.getPosY()   ,0.f,float(height1),0.f,.7f)));
    audioProcessor.audioEngine.updateDelayWetL3((juce::jmap(delayView1.point3.getPosY()   ,0.f,float(height1),0.f,.7f)));
    audioProcessor.audioEngine.updateDelayWetL4((juce::jmap(delayView1.point4.getPosY()   ,0.f,float(height1),0.f,.7f)));
    audioProcessor.audioEngine.updateDelayWetR1((juce::jmap(delayView2.point1.getPosY()   ,0.f,float(height1),0.f,.7f)));
    audioProcessor.audioEngine.updateDelayWetR2((juce::jmap(delayView2.point2.getPosY()   ,0.f,float(height1),0.f,.7f)));
    audioProcessor.audioEngine.updateDelayWetR3((juce::jmap(delayView2.point3.getPosY()   ,0.f,float(height1),0.f,.7f)));
    audioProcessor.audioEngine.updateDelayWetR4((juce::jmap(delayView2.point4.getPosY()   ,0.f,float(height1),0.f,.7f)));
    
    
    audioProcessor.audioEngine.updateDetuneL1( juce::jmap(delayView3.point1.getPosX() ,0.f,float(width1),.01f,1.f));
    audioProcessor.audioEngine.updateDetuneL2((juce::jmap(delayView3.point2.getPosX()  ,0.f,float(width1),.01f,1.f)));
    audioProcessor.audioEngine.updateDetuneL3((juce::jmap(delayView3.point3.getPosX() ,0.f,float(width1),.01f,1.f)));
    audioProcessor.audioEngine.updateDetuneL4((juce::jmap(delayView3.point4.getPosX() ,0.f,float(width1),.01f,1.f)));
    audioProcessor.audioEngine.updateDetuneR1((juce::jmap(delayView4.point1.getPosX() ,0.f,float(width1),.01f,1.f)));
    audioProcessor.audioEngine.updateDetuneR2((juce::jmap(delayView4.point2.getPosX() ,0.f,float(width1),.01f,1.f)));
    audioProcessor.audioEngine.updateDetuneR3((juce::jmap(delayView4.point3.getPosX() ,0.f,float(width1),.01f,1.f)));
    audioProcessor.audioEngine.updateDetuneR4((juce::jmap(delayView4.point4.getPosX()  ,0.f,float(width1),.01f,1.f)));
    
    audioProcessor.audioEngine.updateFreqL1( juce::jmap(delayView3.point1.getPosY() ,0.f,float(height1),.01f,.05f));
    audioProcessor.audioEngine.updateFreqL2((juce::jmap(delayView3.point2.getPosY()  ,0.f,float(height1),.01f,.05f)));
    audioProcessor.audioEngine.updateFreqL3((juce::jmap(delayView3.point3.getPosY() ,0.f,float(height1),.01f,.05f)));
    audioProcessor.audioEngine.updateFreqL4((juce::jmap(delayView3.point4.getPosY() ,0.f,float(height1),.01f,.05f)));
    audioProcessor.audioEngine.updateFreqR1((juce::jmap(delayView4.point1.getPosY() ,0.f,float(height1),.01f,.05f)));
    audioProcessor.audioEngine.updateFreqR2((juce::jmap(delayView4.point2.getPosY() ,0.f,float(height1),.01f,.05f)));
    audioProcessor.audioEngine.updateFreqR3((juce::jmap(delayView4.point3.getPosY() ,0.f,float(height1),.01f,.05f)));
    audioProcessor.audioEngine.updateFreqR4((juce::jmap(delayView4.point4.getPosY()  ,0.f,float(height1),.01f,.05f)));
    
}

void StutterZenAudioProcessorEditor::resized()
{
  
    audioProcessor.audioVisualizer.setBounds(300,100,300,100);
    audioProcessor.audioVisualizer2.setBounds(300,750,300,100);
    thresHoldKnobL.setBounds(50,50,100,100);
    reductionKnobL.setBounds(150,50,100,100);
 
    outGainKnobL.setBounds(50,750,100,100);
    thresHoldKnobR.setBounds(50,150,100,100);
    reductionKnobR.setBounds(150,150,100,100);
   
    outGainKnobR.setBounds(150,750,100,100);
    modeButton.setBounds(550,250,100,100);
    delayView1.setBounds(50,350, width1, height1);
    delayView2.setBounds(50,500, width1, height1);
    delayView3.setBounds(250,350, width1, height1);
    delayView4.setBounds(250,500, width1, height1);
    speedbutton1.setBounds(50, 300, width1, 50);
    speedbutton2.setBounds(250, 300, width1, 50);
    stopbutton1.setBounds(50, 500 + height1, width1, 50);
    stopbutton2.setBounds(250, 500+ height1, width1, 50);
    speedDelayKnob.setBounds(400, 300 + height1 * 0.5, 100, 100);
    speedChorusKnob.setBounds(400, 450+ height1 * 0.5, 100, 100);
    wetDelayKnob.setBounds(500, 300 + height1 * 0.5, 100, 100);
    wetChorusKnob.setBounds(500, 450+ height1 * 0.5, 100, 100);
    resetButton.setBounds(600, 650, 50, 50);
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
void StutterZenAudioProcessorEditor::timerCallback()
{
    repaint();
}

    
