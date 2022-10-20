/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StutterZenAudioProcessor::StutterZenAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),treeState(*this, nullptr, "PARAMETERS", createParameterLayout()),
audioVisualizer(2),
audioVisualizer2(2)
#endif
{
    audioVisualizer.setRepaintRate(30);
    audioVisualizer.setBufferSize(256);
    audioVisualizer2.setRepaintRate(30);
    audioVisualizer2.setBufferSize(256);
    treeState.addParameterListener("deltaL", this);
    treeState.addParameterListener("thresholdL", this);
    treeState.addParameterListener("delayL", this);
    treeState.addParameterListener("delaywetL", this);
    treeState.addParameterListener("gainL", this);
    treeState.addParameterListener("deltaR", this);
    treeState.addParameterListener("thresholdR", this);
    treeState.addParameterListener("delayR", this);
    treeState.addParameterListener("delaywetR", this);
    treeState.addParameterListener("gainR", this);
    treeState.addParameterListener("wetDelay", this);
    treeState.addParameterListener("wetChorus", this);
  //  treeState.addParameterListener("mode", this);
   
}

StutterZenAudioProcessor::~StutterZenAudioProcessor()
{
    treeState.removeParameterListener("deltaL", this);
    treeState.removeParameterListener("thresholdL", this);
    treeState.removeParameterListener("delayL", this);
    treeState.removeParameterListener("delaywetL", this);
    treeState.removeParameterListener("gainL", this);
    treeState.removeParameterListener("deltaR", this);
    treeState.removeParameterListener("thresholdR", this);
    treeState.removeParameterListener("delayR", this);
    treeState.removeParameterListener("delaywetR", this);
    treeState.removeParameterListener("gainR", this);
    treeState.removeParameterListener("wetDelay", this);
    treeState.removeParameterListener("wetChorus", this);
  //  treeState.removeParameterListener("mode", this);
   
}
juce::AudioProcessorValueTreeState::ParameterLayout
StutterZenAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    auto pDeltaL = (std::make_unique<juce::AudioParameterFloat>("deltaL",
                                                          "ReductionL",0.00f,60.f,0.75f));
    auto pThresholdL = (std::make_unique<juce::AudioParameterFloat>("thresholdL",
                                                          "ThresholdL",0,120.f,0.75f));
    auto pOutGainL = (std::make_unique<juce::AudioParameterFloat>("gainL",
                                                          "OutGainL",0,5.f,0.5f));
    auto pDeltaR = (std::make_unique<juce::AudioParameterFloat>("deltaR",
                                                          "ReductionR",0.00f,60.f,0.75f));
    auto pThresholdR = (std::make_unique<juce::AudioParameterFloat>("thresholdR",
                                                          "ThresholdR",0,120.f,0.75f));
    auto pOutGainR = (std::make_unique<juce::AudioParameterFloat>("gainR",
                                                          "OutGainR",0,5.f,0.5f));
    
    auto pMode = (std::make_unique<juce::AudioParameterBool>("mode",
                                                            "Mode",false));
    
    auto pDelayScaleSpeed = (std::make_unique<juce::AudioParameterFloat>("speedDelay",
                                                "SpeedDelay",.25f,1.2f,.5f));
    auto pChorusScaleSpeed = (std::make_unique<juce::AudioParameterFloat>("speedChorus",
                                                "SpeedChorus",.25f,1.2f,.5f));
    auto pDelaywet = (std::make_unique<juce::AudioParameterFloat>("wetDelay",
                                                "SpeedDelay",0.f,1.f,.5f));
    auto pChoruswet = (std::make_unique<juce::AudioParameterFloat>("wetChorus",
                                                "SpeedChorus",0.f,1.f,.25f));
   
    params.push_back(std::move(pDeltaL));
    params.push_back(std::move(pThresholdL));
    params.push_back(std::move(pOutGainL));
    params.push_back(std::move(pDeltaR));
    params.push_back(std::move(pThresholdR));
    params.push_back(std::move(pOutGainR));
    params.push_back(std::move(pMode));
    params.push_back(std::move(pDelayScaleSpeed));
    params.push_back(std::move(pChorusScaleSpeed));
    params.push_back(std::move(pDelaywet));
    params.push_back(std::move(pChoruswet));
    
    return {params.begin(),params.end()};
}

void StutterZenAudioProcessor::parameterChanged(const juce::String &paramterID, float newValue)
{
    if (paramterID == "deltaL")
    {
        audioEngine.updateDeltaL(newValue);
    }
    if (paramterID == "thresholdL")
    {
        audioEngine.updateThresL(newValue);
    }
    if (paramterID == "gainL")
    {
        audioEngine.updateOutputGainL(newValue);
    }
    if (paramterID == "deltaR")
    {
        audioEngine.updateDeltaR(newValue);
    }
    if (paramterID == "thresholdR")
    {
        audioEngine.updateThresR(newValue);
    }
    if (paramterID == "gainR")
    {
        audioEngine.updateOutputGainR(newValue);
    }
    if(paramterID == "wetDelay")
    {
        audioEngine.updateWetDelay(newValue);
    }
    if(paramterID == "wetChorus")
    {
        audioEngine.updateWetChorus(newValue);
    }
  
  
  
}

void StutterZenAudioProcessor::updateParameters()
{
    audioEngine.updateThresL(treeState.getRawParameterValue("thresholdL")->load());
    audioEngine.updateDeltaL(treeState.getRawParameterValue("deltaL")->load());
    audioEngine.updateOutputGainL(treeState.getRawParameterValue("gainL")->load());
    audioEngine.updateThresR(treeState.getRawParameterValue("thresholdR")->load());
    audioEngine.updateDeltaR(treeState.getRawParameterValue("deltaR")->load());
    audioEngine.updateOutputGainR(treeState.getRawParameterValue("gainR")->load());
    audioEngine.updateWetDelay(treeState.getRawParameterValue("wetDelay")->load());
    audioEngine.updateWetChorus(treeState.getRawParameterValue("wetChorus")->load());
    audioVisualizer.updateLineL(treeState.getRawParameterValue("thresholdL")->load());
    audioVisualizer.updateLineR(treeState.getRawParameterValue("thresholdR")->load());
}


//==============================================================================
const juce::String StutterZenAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StutterZenAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StutterZenAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StutterZenAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StutterZenAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StutterZenAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StutterZenAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StutterZenAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StutterZenAudioProcessor::getProgramName (int index)
{
    return {};
}

void StutterZenAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StutterZenAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
        audioEngine.reset();
        juce::dsp::ProcessSpec spec;
        spec.maximumBlockSize = samplesPerBlock;
        spec.sampleRate = sampleRate;
        spec.numChannels = 1;
        audioEngine.prepare(spec);
        updateParameters();
}

void StutterZenAudioProcessor::releaseResources()
{
    audioEngine.reset();
    audioVisualizer.clear();
    audioVisualizer2.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StutterZenAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void StutterZenAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
    audioVisualizer.pushBuffer(buffer);
    audioEngine.renderNextBlock (buffer);
    audioVisualizer2.pushBuffer(buffer);
}

//==============================================================================
bool StutterZenAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StutterZenAudioProcessor::createEditor()
{
    return new StutterZenAudioProcessorEditor (*this);
}

//==============================================================================
void StutterZenAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    
    juce::MemoryOutputStream mos(destData, true);
    treeState.state.writeToStream(mos);
}

void StutterZenAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        treeState.replaceState(tree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StutterZenAudioProcessor();
}
