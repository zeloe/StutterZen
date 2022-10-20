/*
  ==============================================================================

    WaveTable.h
    Created: 18 Oct 2022 10:53:32pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
template <typename Type>
class WaveTable
{
public:
    WaveTable()
    {
        this->fill();
    };
    ~WaveTable(){};
    void clear() noexcept
    {
       // std::fill (rawData.begin(), rawData.end(), Type (0));
    }


    void fill () 
    {
        const size_t waveTableLenght = 512;
        const Type PI = std::atanf(1.f) * 4;
        for (auto i = 0; i < waveTableLenght; ++i)
        {
        this->rawData[i] =std::sin((2.f*PI * (static_cast<Type>(i)/Type(waveTableLenght)))* 0.5f );
        }

    }
    
    void getIncrement (Type frequency,Type sampleRate)
    {
        indexIncrement =511.f/ (static_cast<Type>(sampleRate)/ frequency);
        
    }

    Type getValue ()
    {
        
        
        phase += indexIncrement;
        if ( phase >= 511 ) phase -=511;
        remainder = phase - floor(phase);
        output = (1.f-remainder) * this->rawData[1+ (long) phase] + remainder * this->rawData[2+(long) phase];
        return output;
    }
      

    
private:
    Type rawData [512] {0};
    Type indexIncrement { Type (1) };
    Type phase  { Type (0) };;
    Type output  { Type (0) };;
    Type remainder{Type (0)};
};
