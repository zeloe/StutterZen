/*
  ==============================================================================

    FFeedbackCombDelay.h
    Created: 13 Aug 2022 5:31:09pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#include "JuceHeader.h"
#include "DelayLine.h"
#pragma once
template <typename Type, size_t maxNumChannels = 1>
class Delay
{
public:
    //==============================================================================
    Delay()
    {
        setMaxDelayTime (2.f);
        setWetLevel (1.0f);
        setFeedback (0.00f);
    }
    ~Delay(){};
    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        jassert (spec.numChannels <= maxNumChannels);
                sampleRate = (Type) spec.sampleRate;
                
        ampSmooth.reset(sampleRate,0.00001f);
        delaySmooth1.reset(sampleRate,0.0000001f);
        delaySmooth2.reset(sampleRate,0.0000001f);
        delaySmooth3.reset(sampleRate,0.0000001f);
        delaySmooth4.reset(sampleRate,0.0000001f);
    }

    //==============================================================================
    void reset() noexcept
    {
        delayLine1[0].clear();
        delayLine2[0].clear();
        delayLine3[0].clear();
        delayLine4[0].clear();
    }
    //==============================================================================
    size_t getNumChannels() const noexcept
    {
        return delayLine1.size();
    }

    //==============================================================================
    void setMaxDelayTime (Type newValue)
    {
        jassert (newValue > Type (0));
        maxDelayTime = newValue;
        delayLine1[0].resize (maxDelayTime * sampleRate);
        delayLine2[0].resize (maxDelayTime * sampleRate);
        delayLine3[0].resize (maxDelayTime * sampleRate);
        delayLine4[0].resize (maxDelayTime * sampleRate);
    }

    //==============================================================================
    void setFeedback (Type newValue) noexcept
    {
        jassert (newValue >= Type (0) && newValue <= Type (1));
        feedback = newValue;
    }

    //==============================================================================
    void setWetLevel (Type newValue) noexcept
    {
        jassert (newValue >= Type (0) && newValue <= Type (1));
        wetLevel = newValue;
    }

    //==============================================================================
    void setDelayTime (size_t channel, Type newValue)
    {
        if (channel >= getNumChannels())
        {
            jassertfalse;
            return;
        }

        jassert (newValue >= Type (0));
        delayTime = newValue;

    }
    void setAmount (Type newValue)
    {
        ampSmooth.setTargetValue(newValue);
    }
    
    void setTime (Type newValue)
    {
        time = newValue;
    }
    //==============================================================================
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        auto& inputBlock  = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();
        auto numSamples  = outputBlock.getNumSamples();
        auto numChannels = outputBlock.getNumChannels();

        jassert (inputBlock.getNumSamples() == numSamples);
        jassert (inputBlock.getNumChannels() == numChannels);


        for (size_t ch = 0; ch < numChannels; ++ch)
        {
            auto* input  = inputBlock .getChannelPointer (ch);
            auto* output = outputBlock.getChannelPointer (ch);
            
            
            for (size_t i = 0; i < numSamples; ++i)
                   {
                       delaySmooth1.setTargetValue((0.1 +delayTime) * sampleRate);
                       delaySmooth2.setTargetValue((0.2 +delayTime) * sampleRate);
                       delaySmooth3.setTargetValue((0.3 +delayTime) * sampleRate);
                       delaySmooth4.setTargetValue((0.4 +delayTime) * sampleRate);
                       auto delayedSample1 = (delayLine1[0].get(delaySmooth1.getNextValue()));
                       auto delayedSample2 = (delayLine2[0].get (delaySmooth2.getNextValue()));
                       auto delayedSample3 = (delayLine3[0].get (delaySmooth3.getNextValue()));
                       auto delayedSample4 = (delayLine4[0].get (delaySmooth4.getNextValue()));
                       auto inputSample = input[i];
                       delayLine1[0].push (inputSample);
                       delayLine2[0].push (inputSample);
                       delayLine3[0].push (inputSample);
                       delayLine4[0].push (inputSample);
                       auto outputSample =  (delayedSample1 + delayedSample2 + delayedSample3 + delayedSample4) * ampSmooth.getNextValue() + (ampSmooth.getNextValue() -1.f) * inputSample;
                       
                       output[i] = outputSample;
                      
                   }
        }
    }

private:
    //==============================================================================
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear>  ampSmooth {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth1 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth2 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth3 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth4 {0.0f};
    std::array<DelayLine<Type>, maxNumChannels> delayLine1;
    std::array<DelayLine<Type>, maxNumChannels> delayLine2;
    std::array<DelayLine<Type>, maxNumChannels> delayLine3;
    std::array<DelayLine<Type>, maxNumChannels> delayLine4;
    std::array<size_t, maxNumChannels> delayTimesSample;
    std::array<Type, maxNumChannels> delayTimes;
    Type feedback { Type (0) };
    Type wetLevel { Type (0) };
    Type time { Type (0) };
    Type amount {Type (0)};
    Type delayTime {Type (0)};
    Type sampleRate   { Type (44.1e3) };
    Type maxDelayTime { Type (2) };
    Type delayTime1 { Type (2) };
    Type delayTime2 { Type (2) };
    Type delayTime3 { Type (2) };
    Type delayTime4 { Type (2) };
    //==============================================================================
   
    //==============================================================================
};
