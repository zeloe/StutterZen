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
        
        for(auto& dl : delayLines)
        {
            dl.prepare(spec);
        }
        updateParameters();
        updateAmount();
        
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
        delayLines[0].process(context);
    }
    void getAmount (Type newValue)
    {
        amount = newValue;
        updateAmount();
    }
   
    void getTime (Type newValue)
    {
    delayLines[0].setDelayTime(0,newValue);
      
    }
private:
    Type amount {Type (0)};
    Type time {Type (0)};
    std::array<Delay<Type>, 1> delayLines ;
    void updateParameters()
    {
        
            delayLines[0].setWetLevel(1.0f);
    }
       
    void updateAmount()
    {
        
            delayLines[0].setAmount(amount);
        
    }
    
    
};
