/*
  ==============================================================================

    ChorusProcess.h
    Created: 15 Apr 2022 2:18:17pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "DelayLine.h"
#include "WaveTable.h"
template <typename Type, size_t maxNumChannels = 1>
class ChorusProcess
{
public:
    ChorusProcess()
    {
        setMaxDelayTime (3.f);
    }
    ~ChorusProcess()
    {
        
    }
    
   
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        jassert (spec.numChannels <= maxNumChannels);
                sampleRate = (Type) spec.sampleRate;
        sampleRate = (Type) spec.sampleRate;
        ampSmooth1.reset(sampleRate,0.001f);
        delaySmooth1.reset(sampleRate,1.f);
        delaySmooth2.reset(sampleRate,1.f);
        delaySmooth3.reset(sampleRate,1.f);
        delaySmooth4.reset(sampleRate,1.f);
        freqSmooth1.reset(sampleRate,.001f);
        freqSmooth2.reset(sampleRate,.001f);
        freqSmooth2.reset(sampleRate,.001f);
        freqSmooth2.reset(sampleRate,.001f);
    }
    void reset() noexcept
    {
        xdelayLine1[0].clear();
        xdelayLine2[0].clear();
        xdelayLine3[0].clear();
        xdelayLine4[0].clear();
    }
    void setMaxDelayTime (Type newValue)
    {
        jassert (newValue > Type (0));
        maxDelayTime = newValue;
        xdelayLine1[0].resize (maxDelayTime * sampleRate);
        xdelayLine2[0].resize (maxDelayTime * sampleRate);
        xdelayLine3[0].resize (maxDelayTime * sampleRate);
        xdelayLine4[0].resize (maxDelayTime * sampleRate);
    }
    size_t getNumChannels() const noexcept
    {
        return xdelayLine1[0].size();
    }
 
    void setFreq1 (Type newValue)
    {
        freqSmooth1.setTargetValue(newValue);
        waveTables[0].getIncrement(freqSmooth1.getNextValue(),sampleRate);
    }
    
    void setFreq2 (Type newValue)
    {
        freqSmooth2.setTargetValue(newValue);
        waveTables[1].getIncrement(freqSmooth2.getNextValue(),sampleRate);
    }
    
    void setFreq3 (Type newValue)
    {
        freqSmooth3.setTargetValue(newValue);
        waveTables[2].getIncrement(freqSmooth3.getNextValue(),sampleRate);
    }
    
    void setFreq4 (Type newValue)
    {
        freqSmooth4.setTargetValue(newValue);
        waveTables[3].getIncrement(freqSmooth4.getNextValue(),sampleRate);
    }
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
                
                xdelayLine1[0].getTime(delaySmooth1.getNextValue()* sampleRate *0.01f* waveTables[0].getValue() * 5.f +1.f);
                xdelayLine2[0].getTime(delaySmooth1.getNextValue()* sampleRate *0.01f *  waveTables[1].getValue()* 5.f +1.f);
                xdelayLine3[0].getTime(delaySmooth1.getNextValue()* sampleRate *0.01f *  waveTables[2].getValue()* 5.f +1.f);
                xdelayLine4[0].getTime(delaySmooth1.getNextValue()* sampleRate *0.01f *  waveTables[3].getValue()* 5.f +1.f);
                auto temp1 =  xdelayLine1[0].get(inputSample);
                auto temp2 = xdelayLine2[0].get(inputSample);
                auto temp3 = xdelayLine3[0].get(inputSample);
                auto temp4 = xdelayLine4[0].get(inputSample);
                auto outputSample =  temp1 + temp2 + temp3 + temp4;
                output[i] =ampSmooth1.getNextValue()*outputSample + (1.f - ampSmooth1.getNextValue()) * inputSample;
                 
          //  output[i] = input[i];
          }
        }
    }
        void setDelayTime1 (size_t channel, Type newValue)
        {
           
            delayTime1 = newValue;
            delaySmooth1.setTargetValue(newValue);
           

        }
        void setDelayTime2 (size_t channel, Type newValue)
        {
            
            delayTime2 = newValue;
            delaySmooth2.setTargetValue(newValue);
           
        }
        void setDelayTime3 (size_t channel, Type newValue)
        {
           

          
            delayTime3 = newValue;
            delaySmooth3.setTargetValue(newValue);
          

        }
        void setDelayTime4 (size_t channel, Type newValue)
        {
            
            delayTime4 = newValue;
            delaySmooth4.setTargetValue(newValue);
           
        }
        
        void setAmount1 (float newValue)
        {
            ampSmooth1.setTargetValue(newValue);
        }
    

private:
        Type delayTime1 {Type (0)};
        Type delayTime2 {Type (0)};
        Type delayTime3 {Type (0)};
        Type delayTime4 {Type (0)};
        Type freq1 {Type (0)};
        Type freq2 {Type (0)};
        Type freq3 {Type (0)};
        Type freq4 {Type (0)};
        Type output2{Type (0)};
        Type sampleRate {Type (44100)};
        Type maxDelayTime {Type (3)};
        Type index {Type (0)};
    std::array<DelayLine<Type>, maxNumChannels> xdelayLine1;
    std::array<DelayLine<Type>, maxNumChannels> xdelayLine2;
    std::array<DelayLine<Type>, maxNumChannels> xdelayLine3;
    std::array<DelayLine<Type>, maxNumChannels> xdelayLine4;
    std::array<WaveTable<Type>,4> waveTables;
   
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth1 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth2 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth3 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> delaySmooth4 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> freqSmooth1 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> freqSmooth2 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> freqSmooth3 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> freqSmooth4 {0.0f};
    juce::SmoothedValue<Type, juce::ValueSmoothingTypes::Linear> ampSmooth1 {0.0f};
};

