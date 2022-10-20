/*
  =============================================================================

    AudioProcessor.
    Created: 13 Aug 2022 3:22:32p
    Author:  Zeno  Loesc

  =============================================================================
*/

#pragma onc
#include "Gate_L.h"
#include "PreDelays.h"
#include "Chorus.h"
class AudioEngine
{
public:
    AudioEngine() {}
    ~AudioEngine() {}
   
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept
    {
        ampSmooth1.reset(spec.sampleRate, 0.0001f);
        ampSmooth2.reset(spec.sampleRate, 0.0001f);
        fxChainL.prepare(spec);
        fxChainR.prepare(spec);
    }
    void reset()
    {
    
       
        fxChainL.reset();
        fxChainR.reset();
    }
    void renderNextBlock (juce::AudioBuffer<float>& outputAudio)
    {
        juce::dsp::AudioBlock<float> block(outputAudio);
       
        auto blockL = block.getSingleChannelBlock(0);
        auto blockR = block.getSingleChannelBlock(1);
    
        juce::dsp::ProcessContextReplacing<float> contextL (blockL);
        fxChainL.process(contextL);
        juce::dsp::ProcessContextReplacing<float> contextR(blockR);
        fxChainR.process(contextR);
    }
    void updateDeltaL (float newValue)
    {
    
        fxChainL.template get<0>().setDelta(newValue);
        
    }
    void updateThresL (float newValue)
    {
    
        fxChainL.template get<0>().setThreshold(newValue);
       
    }
    
    void updateDelayL1 (float newValue)
    {
    
        fxChainL.template get<1>().getTime1(newValue);
       
    }
    void updateDelayL2 (float newValue)
    {
    
        fxChainL.template get<1>().getTime2(newValue);
       
    }
    void updateDelayL3 (float newValue)
    {
    
        fxChainL.template get<1>().getTime3(newValue);
       
    }
    void updateDelayL4 (float newValue)
    {
    
        fxChainL.template get<1>().getTime4(newValue);
       
    }
    
    void updateDelayWetL1 (float newValue)
    {
    
        fxChainL.template get<1>().getAmount1(newValue);
       
    }
    void updateDelayWetL2 (float newValue)
    {
    
        fxChainL.template get<1>().getAmount2(newValue);
       
    }
    void updateDelayWetL3 (float newValue)
    {
    
        fxChainL.template get<1>().getAmount3(newValue);
       
    }
    void updateDelayWetL4 (float newValue)
    {
    
        fxChainL.template get<1>().getAmount4(newValue);
       
    }
    void updateDeltaR (float newValue)
    {
        fxChainR.template get<0>().setDelta(newValue);
    }
    void updateThresR (float newValue)
    {
    
       
        fxChainR.template get<0>().setThreshold(newValue);
    }
    
    void updateDelayR1(float newValue)
    {
    
        
        fxChainR.template get<1>().getTime1(newValue);
    }
    
    void updateDelayR2(float newValue)
    {
    
        
        fxChainR.template get<1>().getTime2(newValue);
    }
    
    void updateDelayR3 (float newValue)
    {
    
        
        fxChainR.template get<1>().getTime3(newValue);
    }
    
    void updateDelayR4 (float newValue)
    {
    
        
        fxChainR.template get<1>().getTime4(newValue);
    }
    
    void updateDelayWetR1(float newValue)
    {
    
        fxChainR.template get<1>().getAmount1(newValue);
    }
    
    void updateDelayWetR2 (float newValue)
    {
    
        fxChainR.template get<1>().getAmount2(newValue);
    }
    void updateDelayWetR3(float newValue)
    {
    
        fxChainR.template get<1>().getAmount3(newValue);
    }
    
    void updateDelayWetR4(float newValue)
    {
    
        fxChainR.template get<1>().getAmount4(newValue);
    }
   
    
    
    
    void updateDetuneL1 (float newValue)
    {
    
        fxChainL.template get<2>().getTime1(newValue);
       
    }
    void updateDetuneL2(float newValue)
    {
    
        fxChainL.template get<2>().getTime2(newValue);
       
    }
    void updateDetuneL3 (float newValue)
    {
    
        fxChainL.template get<2>().getTime3(newValue);
       
    }
    void updateDetuneL4 (float newValue)
    {
    
        fxChainL.template get<2>().getTime4(newValue);
       
    }
    
    void updateDetuneR1 (float newValue)
    {
    
        fxChainR.template get<2>().getTime1(newValue);
       
    }
    void updateDetuneR2(float newValue)
    {
    
        fxChainR.template get<2>().getTime2(newValue);
       
    }
    void updateDetuneR3 (float newValue)
    {
    
        fxChainR.template get<2>().getTime3(newValue);
       
    }
    void updateDetuneR4 (float newValue)
    {
    
        fxChainR.template get<2>().getTime4(newValue);
       
    }
    
    
    void updateOutputGainL(float newValue)
    {
        ampSmooth1.setTargetValue(newValue);
        auto& gainL = fxChainL.template get<3>();
        gainL.setGainLinear (ampSmooth1.getNextValue());
        
    }
    void updateOutputGainR(float newValue)
    {
        ampSmooth2.setTargetValue(newValue);
        auto& gainR = fxChainR.template get<3>();
        gainR.setGainLinear (ampSmooth2.getNextValue());
    }
   
    void updateWetDelay(float newValue)
    {
        fxChainL.template get<1>().getWet(newValue);
        fxChainR.template get<1>().getWet(newValue);
    }
    
    void updateWetChorus(float newValue)
    {
        fxChainL.template get<2>().getAmount1(newValue);
        fxChainR.template get<2>().getAmount1(newValue);
    }
    
    void updateFreqL1(float newValue)
    {
        fxChainL.template get<2>().getFreq1(newValue);
    }
    void updateFreqL2(float newValue)
    {
        fxChainL.template get<2>().getFreq2(newValue);
    }
    void updateFreqL3(float newValue)
    {
        fxChainL.template get<2>().getFreq3(newValue);
    }
    void updateFreqL4(float newValue)
    {
        fxChainL.template get<2>().getFreq4(newValue);
    }
    
    void updateFreqR1(float newValue)
    {
        fxChainR.template get<2>().getFreq1(newValue);
    }
    void updateFreqR2(float newValue)
    {
        fxChainR.template get<2>().getFreq2(newValue);
    }
    void updateFreqR3(float newValue)
    {
        fxChainR.template get<2>().getFreq3(newValue);
    }
    void updateFreqR4(float newValue)
    {
        fxChainR.template get<2>().getFreq4(newValue);
    }
   
private:
   
  //  juce::dsp::ProcessorChain<Gate_L<float>> fxChain;
    juce::dsp::ProcessorChain<Gate_L<float>,PreDelays<float>,Chorus<float>,juce::dsp::Gain<float>> fxChainL;
    juce::dsp::ProcessorChain<Gate_L<float>,PreDelays<float>,Chorus<float>,juce::dsp::Gain<float>> fxChainR;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> ampSmooth1 {0.0f};
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> ampSmooth2 {0.0f};
  
   
   
};

