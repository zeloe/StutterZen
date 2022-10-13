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
class AudioEngine
{
public:
    AudioEngine() {}
    ~AudioEngine() {}
   
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept
    {
    
      
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
    
        //auto block = juce::dsp::AudioBlock<float> (outputAudio).getSubBlock ((size_t) startSample, (size_t) numSamples)
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
    
    void updateDelayL (float newValue)
    {
    
        fxChainL.template get<1>().getTime(newValue);
       
    }
    
    void updateDelayWetL (float newValue)
    {
    
        fxChainL.template get<1>().getAmount(newValue);
       
    }
    void updateDeltaR (float newValue)
    {
    
         
        fxChainR.template get<0>().setDelta(newValue);
    }
    void updateThresR (float newValue)
    {
    
       
        fxChainR.template get<0>().setThreshold(newValue);
    }
    
    void updateDelayR (float newValue)
    {
    
        
        fxChainR.template get<1>().getTime(newValue);
    }
    
    void updateDelayWetR (float newValue)
    {
    
        fxChainR.template get<1>().getAmount(newValue);
    }
   
    void updateOutputGainL(float newValue)
    {
        
        auto& gainL = fxChainL.template get<2>();
        gainL.setGainLinear (newValue);
        
    }
    void updateOutputGainR(float newValue)
    {
        
        auto& gainR = fxChainR.template get<2>();
        gainR.setGainLinear (newValue);
    }
  
   
private:
   
  //  juce::dsp::ProcessorChain<Gate_L<float>> fxChain;
    juce::dsp::ProcessorChain<Gate_L<float>,PreDelays<float>,juce::dsp::Gain<float>> fxChainL;
    juce::dsp::ProcessorChain<Gate_L<float>,PreDelays<float>,juce::dsp::Gain<float>> fxChainR;
   
  
   
   
};

