/*
  ==============================================================================

    MoorerReverb.h
    Created: 13 Aug 2022 2:30:59pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Delay.h"


template <typename Type>
class PreDelays
{
public:
    PreDelays(){};
    ~PreDelays(){};
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        delaySmooth.reset(spec.sampleRate,0.001f);
        for(auto& dl : delayLines)
        {
            dl.prepare(spec);
        }
        updateParameters();
        updateAmount();
        updateTime();
        
    }
    void reset() noexcept
    {
        for(auto& dl : delayLines)
        {
            dl.reset();
        }
    }

template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept
    {
        for(auto& dl : delayLines)
        {
            dl.process(context);
        }
    }
    void getAmount (Type newValue)
    {
        amount = newValue;
        updateAmount();
    }
   
    void getTime (Type newValue)
    {
       delaySmooth.setTargetValue(newValue);
        updateTime();
    }
private:
    juce::LinearSmoothedValue<Type> delaySmooth {0.0f};
    Type amount {Type (0)};
    Type time {Type (0)};
    std::array<Delay<Type>, 4> delayLines ;
    const float preDelayParams [5]
    {0.25,0.5,1.0,1.5
    };
    
    const float preDelayAmpParams [5]
    {1.00,1.00,1.0,1.0
    };
 
    void updateParameters()
    {
        size_t counter2 = 0;
        for(auto& dl : delayLines)
        {
            dl.setWetLevel(preDelayAmpParams[counter2]);
            counter2++;
            
        }
       
    }
    void updateAmount()
    {
        size_t counter3 = 0;
        for(auto& dl : delayLines)
        {
            dl.setAmount(amount);
            counter3++;
        }
        
    }
    void updateTime()
    {
        size_t counter3 = 0;
        for(auto& dl : delayLines)
        {
            dl.setDelayTime(0,preDelayParams[counter3] * delaySmooth.getNextValue());
            
            counter3++;
            
        }
        
    }
    
};
