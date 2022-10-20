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
    void getAmount1(Type newValue)
    {
            delayLines[0].setAmount1(newValue);
    }
    void getAmount2(Type newValue)
    {
            delayLines[0].setAmount2(newValue);
    }
    void getAmount3(Type newValue)
    {
            delayLines[0].setAmount3(newValue);
    }
    void getAmount4(Type newValue)
    {
            delayLines[0].setAmount4(newValue);
    }
    
    void getWet(Type newValue)
    {
        delayLines[0].setWet(newValue);
    }
    
    void getTime1 (Type newValue)
    {
    delayLines[0].setDelayTime1(0,newValue);
      
    }
    void getTime2 (Type newValue)
    {
    delayLines[0].setDelayTime2(0,newValue);
      
    }
    void getTime3 (Type newValue)
    {
    delayLines[0].setDelayTime3(0,newValue);
      
    }
    void getTime4 (Type newValue)
    {
    delayLines[0].setDelayTime4(0,newValue);
      
    }
   
private:
    Type amount {Type (0)};
    Type time {Type (0)};
    std::array<Delay<Type>, 1> delayLines ;
    void updateParameters()
    {
        
            delayLines[0].setWetLevel(1.0f);
    }
       
    
    
    
};
