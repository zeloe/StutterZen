/*
  ==============================================================================

    Chourus.h
    Created: 14 Oct 2022 10:22:01pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once


#pragma once
#include "JuceHeader.h"
#include "ChorusProcess.h"


template <typename Type>
class Chorus
{
public:
    Chorus(){};
    ~Chorus(){};
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        
        for(auto& dl : delayLines)
        {
            dl.prepare(spec);
        }
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
    void getAmount1(Type newValue)
    {
            delayLines[0].setAmount1(newValue);
    }
    
    void getFreq1 (Type newValue)
    {
    delayLines[0].setFreq1(newValue);
      
    }
    void getFreq2 (Type newValue)
    {
    delayLines[0].setFreq2(newValue);
      
    }
    void getFreq3 (Type newValue)
    {
    delayLines[0].setFreq3(newValue);
      
    }
    void getFreq4 (Type newValue)
    {
    delayLines[0].setFreq4(newValue);
      
    }
    
    
    
    
private:
    std::array<ChorusProcess<Type>, 1> delayLines ;
};
