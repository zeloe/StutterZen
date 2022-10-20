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
        wptr = 0;
        rptr = 0;
    }

    Type back()
    {
    }
    
    void getTime(Type delayInSamples)
    {
        jassert (delayInSamples >= 0.f && delayInSamples < size());
        delay  = juce::jlimit ((Type) 0, Type(size()), delayInSamples);
        
    }
    
    Type get (Type input)
    {
        
        
        if (wptr >= size()) wptr %= size();
        rawData[wptr] = input;
        
        rptr1 = wptr +static_cast<size_t> (std::floor (delay));
        rptr2 = rptr1 + 1;
        if (rptr2 >= size())
        {
            rptr1  %= size();
            rptr2 %= size();
            rptr %= size();
        }
        Type a = rawData[rptr1];
        Type b = rawData[rptr2];
        Type t = delay - Type(static_cast<int> (std::floor (delay)));
        ++wptr;
        return  a + (b - a) * t;
        
        
      
        }
      

    /** Adds a new value to the delay line, overwriting the least recently added sample */
    void push (Type valueToAdd) noexcept
    {
        rawData[(wptr)] = valueToAdd;
       // delayInt = delayInt == 0 ? size() - 1 : delayInt - 1;   // [2]
    }

private:
    std::vector<Type> rawData;
    size_t g = 2;
    Type Delay { Type (0) };
    //rptr = 0;
   // Type rptr { Type (0) };
    size_t rptr =0 ;
    size_t rptr1 =0 ;
    size_t rptr2 =0 ;
    //size_t rptr = 0;
    size_t wptr = 0;
    Type delayFrac { Type (0) };
    Type delay { Type (0) };
    Type value1 { Type (0) };
    Type value2 { Type (0) };
    Type output { Type (0) };
   // Type wptr { Type (0) };
    size_t delayInt2 = 0;
    size_t rpi = 0;
    size_t leastRecentIndex = 0;
};
