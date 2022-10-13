/*
  ==============================================================================

    DelayLine.h
    Created: 6 Aug 2022 3:12:17pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
template <typename Type>
class DelayLine
{
public:
    DelayLine(){};
    ~DelayLine(){};
    void clear() noexcept
    {
        std::fill (rawData.begin(), rawData.end(), Type (0));
    }

    size_t size() const noexcept
    {
        return rawData.size();
    }

    void resize (size_t newValue)
    {
        rawData.resize (newValue);
        delayInt = 0;
    }

    Type back() const noexcept
    {
        return rawData[delayInt];
    }

    Type get (Type delayInSamples) 
    {
        jassert (delayInSamples >= 0 && delayInSamples < size());
        delay = juce::jlimit(static_cast<Type>(0),static_cast<Type>( size()), (delayInSamples));
        delayInt2  = static_cast<size_t> (std::floor (delay));
        delayFrac = delay - static_cast<Type>(delayInt2);
        
        index1 = delayInt;
        index2 = index1 + 1;

        if (index2 >= size())
            {
                index1 %=  size();
                index2 %=  size();
            }

        value1 = rawData[(index1)];
        value2 = rawData[(index2)];
        //(1 - t) * v0 + t * v1;
        return (1.f - delayFrac)* value1 +  (delayFrac * value2);
        
        
    }

    /** Set the specified sample in the delay line */
    void set (size_t delayInSamples, Type newValue) noexcept
    {
        jassert (delayInSamples >= 0 && delayInSamples < size());
       // rawData[(delayInt + 1) % size()] = newValue; // [4]
    }

    /** Adds a new value to the delay line, overwriting the least recently added sample */
    void push (Type valueToAdd) noexcept
    {
        rawData[(delayInt + 1) % size()] = valueToAdd;
        delayInt = delayInt == 0 ? size() - 1 : delayInt - 1;   // [2]
    }

private:
    std::vector<Type> rawData;
    Type delayFrac {0.0};
    Type delay {0.0};
    Type value1 {0.0};
    Type value2 {0.0};
    size_t index1 = 0;
    size_t index2 = 0;
    size_t delayInt = 0;
    size_t delayInt2 = 0;
    size_t leastRecentIndex = 0;
};
