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
        setMaxDelayTime (1.f);
        setWetLevel (1.0f);
        setFeedback (0.00f);
    }
    ~Delay(){};
    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        jassert (spec.numChannels <= maxNumChannels);
                sampleRate = (Type) spec.sampleRate;
        wetSmooth1.reset(sampleRate,0.001f);
        ampSmooth1.reset(sampleRate,0.001f);
        ampSmooth2.reset(sampleRate,0.001f);
        ampSmooth3.reset(sampleRate,0.001f);
        ampSmooth4.reset(sampleRate,0.001f);
        delaySmooth1.reset(sampleRate,.1f);
        delaySmooth2.reset(sampleRate,.1f);
        delaySmooth3.reset(sampleRate,.1f);
        delaySmooth4.reset(sampleRate,.1f);
        
    }

    //==============================================================================
    void reset() noexcept
    {
        delayLine1[0].clear();
        delayLine2[0].clear();
        delayLine3[0].clear();
        delayLine4[0].clear();
        fdelayLine1[0].clear();
        fdelayLine2[0].clear();
        fdelayLine3[0].clear();
        fdelayLine4[0].clear();
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
        fdelayLine1[0].resize (maxDelayTime * sampleRate);
        fdelayLine2[0].resize (maxDelayTime * sampleRate);
        fdelayLine3[0].resize (maxDelayTime * sampleRate);
        fdelayLine4[0].resize (maxDelayTime * sampleRate);
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
    void setDelayTime1 (size_t channel, Type newValue)
    {
        if (channel >= getNumChannels())
        {
            jassertfalse;
            return;
        }

        jassert (newValue >= Type (0));
        delaySmooth1.setTargetValue(newValue);
        delayLine1[0].getTime(delaySmooth1.getNextValue()* sampleRate *0.01f);
        fdelayLine1[0].getTime(delaySmooth1.getNextValue()* sampleRate *0.01f);
    }
    void setDelayTime2 (size_t channel, Type newValue)
    {
        if (channel >= getNumChannels())
        {
            jassertfalse;
            return;
        }

        jassert (newValue >= Type (0));
        delaySmooth2.setTargetValue(newValue);
        delayLine2[0].getTime(delaySmooth2.getNextValue()* sampleRate *0.01f);
        fdelayLine2[0].getTime(delaySmooth2.getNextValue()* sampleRate *0.01f);

    }
    void setDelayTime3 (size_t channel, Type newValue)
    {
        if (channel >= getNumChannels())
        {
            jassertfalse;
            return;
        }

        jassert (newValue >= Type (0));
        delaySmooth3.setTargetValue(newValue);
        delayLine3[0].getTime(delaySmooth3.getNextValue()* sampleRate *0.01f);
        fdelayLine3[0].getTime(delaySmooth3.getNextValue()* sampleRate *0.01f);

    }
    void setDelayTime4 (size_t channel, Type newValue)
    {
        if (channel >= getNumChannels())
        {
            jassertfalse;
            return;
        }

        jassert (newValue >= Type (0));
        delaySmooth4.setTargetValue(newValue);
        delayLine4[0].getTime(delaySmooth4.getNextValue()* sampleRate *0.01f);
        fdelayLine4[0].getTime(delaySmooth4.getNextValue()* sampleRate *0.01f);
    }
    void setAmount1 (Type newValue)
    {
        ampSmooth1.setTargetValue(newValue);
    }
    void setAmount2 (Type newValue)
    {
        ampSmooth2.setTargetValue(newValue);
    }
    void setAmount3 (Type newValue)
    {
        ampSmooth3.setTargetValue(newValue);
    }
    void setAmount4 (Type newValue)
    {
        ampSmooth4.setTargetValue(newValue);
    }
    void setWet (Type newValue)
    {
        wetSmooth1.setTargetValue(newValue);
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
                      
                       
                      
                       auto inputSample = input[i];
                       auto delayedSample1 = delayLine1[0].get(inputSample *ampSmooth1.getNextValue());
                       auto delayedSample2 = delayLine2[0].get(inputSample  *ampSmooth2.getNextValue()   );
                       auto delayedSample3 = delayLine3[0].get(inputSample  *ampSmooth3.getNextValue()   );
                       auto delayedSample4 = delayLine4[0].get(inputSample  *ampSmooth4.getNextValue()   );
                       auto fdelayedSample1 = fdelayLine1[0].get(temp1  *ampSmooth1.getNextValue()   );
                       auto fdelayedSample2 = fdelayLine2[0].get(temp2   *ampSmooth2.getNextValue()  );
                       auto fdelayedSample3 = fdelayLine3[0].get(temp3    *ampSmooth3.getNextValue() );
                       auto fdelayedSample4 = fdelayLine4[0].get(temp4    *ampSmooth4.getNextValue() );
                       
                        temp1 = delayedSample1 + fdelayedSample1;
                        temp2 = delayedSample2 + fdelayedSample2;
                        temp3 = delayedSample3 + fdelayedSample3;
                        temp4 = delayedSample4 + fdelayedSample4;
                       auto outputSample =  (temp1 + temp2 + temp3 +temp4) * wetSmooth1.getNextValue()+(1.f - wetSmooth1.getNextValue()) * inputSample;
                       output[i] = outputSample;
                
                //      auto inputSample = input[i];
                //       output[i] = inputSample;
                   }
        }
    }

private:
    //==============================================================================
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear>  ampSmooth1 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear>  ampSmooth2 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear>  ampSmooth3 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear>  ampSmooth4 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth1 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth2 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth3 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth4 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear>  wetSmooth1 {0.0f};
    std::array<DelayLine<Type>, maxNumChannels> delayLine1;
    std::array<DelayLine<Type>, maxNumChannels> delayLine2;
    std::array<DelayLine<Type>, maxNumChannels> delayLine3;
    std::array<DelayLine<Type>, maxNumChannels> delayLine4;
    std::array<DelayLine<Type>, maxNumChannels> fdelayLine1;
    std::array<DelayLine<Type>, maxNumChannels> fdelayLine2;
    std::array<DelayLine<Type>, maxNumChannels> fdelayLine3;
    std::array<DelayLine<Type>, maxNumChannels> fdelayLine4;
    std::array<size_t, maxNumChannels> delayTimesSample;
    std::array<Type, maxNumChannels> delayTimes;
    Type feedback { Type (0) };
    Type wetLevel { Type (0) };
    Type time { Type (0) };
    Type amount {Type (0)};
    Type delayTime1 {Type (0)};
    Type delayTime2{Type (0)};
    Type delayTime3 {Type (0)};
    Type delayTime4{Type (0)};
    Type sampleRate   { Type (44.1e3) };
    Type maxDelayTime { Type (2) };
    Type temp1 {Type (0)};
    Type temp2{Type (0)};
    Type temp3 {Type (0)};
    Type temp4{Type (0)};
    //==============================================================================
   
    //==============================================================================
};
