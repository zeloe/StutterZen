/*
  ==============================================================================

    Gate_L.h
    Created: 3 Oct 2022 2:26:03pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
template <typename Type>
class Gate_L
{
public:
    Gate_L()
    {
        
    }
    ~Gate_L()
    {
        
    }
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = (Type) spec.sampleRate;
        thresSmooth.reset(sampleRate,0.00001f);
        deltaSmooth.reset(sampleRate,0.00001f);
    }
    
    void setThreshold (Type newValue) noexcept
    {
        thresSmooth.setTargetValue(newValue);
    }
    void reset () noexcept
    {
        
    }
    //==============================================================================
    void setDelta (Type newValue) noexcept
    {
        deltaSmooth.setTargetValue(newValue);
    }
    
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        auto& inputBlock  = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();
        auto numSamples  = outputBlock.getNumSamples();
        auto numChannels = outputBlock.getNumChannels();
        // channel Indexing is worng
        for (size_t ch = 0; ch < numChannels; ++ch)
        {
        auto* input  = inputBlock .getChannelPointer (ch);
        auto* output = outputBlock.getChannelPointer (ch);
        for (size_t i = 0; i < numSamples; ++i)
            {
                auto input_common = input[i];
                if (input_common < 0.0) {   //  Calculate absolute value of side
                                            //  chain input signal
                    abs_common = -input_common;
                }
                else {
                    abs_common = input_common;
                }

                abs_common_alpha_beta = abs_common*AlphaBeta;
                abs_common_alpha_beta_minus_peak_value = abs_common_alpha_beta - peak_value_old;

                if (abs_common_alpha_beta_minus_peak_value < 0.0) {
                    abs_common_alpha_beta_minus_peak_value = 0.0;
                }

                abs_common_alpha = abs_common_alpha_beta_minus_peak_value*Alpha;
                abs_common_alpha_plus_peak_value = abs_common_alpha + peak_value_old;
                peak_value_old_beta = peak_value_old*Beta;
                abs_minus_peak_value_old_beta = abs_common_alpha_plus_peak_value - peak_value_old_beta;

                peak_value  = abs_minus_peak_value_old_beta;

                peak_value_old = peak_value;   // Remember peak value for next time

                //  Level Processor
                if (peak_value < min_threshold) {
                    peak_value = min_threshold;
                }

                level_db = log10f(peak_value)*20.0; // Calculate level in Decibel
                level_db -= -thresSmooth.getNextValue();
                if (level_db < 0.0) {      //  If Level (dB) is under threshold
                    level_db = 0.0;        //  don't touch it
                }
                level_db *= -deltaSmooth.getNextValue();         //  Calculate level reduction
                level_db *= 0.05;          //  Avoid division
        //      level_db = level_db/20.0;
                level = powf(10.0, level_db);   // Delog level from Decibel

        //  Gain Smoothing
                hysteresis_in = level_old - level;
                level_old = level;

        //  Hysteresis to avoid "zipper" noise
                if (hysteresis_in > 0.001) {
                    hysteresis_gamma_delta = 0.0046;
                }
                else if (hysteresis_in < -0.001) {
                    hysteresis_gamma_delta = 0.000458;
                }

        //   Calculate output gain with hysteresis output
                hysteresis_out = hysteresis_gamma_delta*(level - gain_old);
                gain = hysteresis_out + gain_old;
                gain_old = gain;

        //   Output the audio signals with calculated gain and "hub" to compensate level reduction
                output[i] = input[i] * gain * hub;
            }
        }
        
    }
    
private:
    //==============================================================================
    juce::LinearSmoothedValue<Type> thresSmooth {0.0f};
    juce::LinearSmoothedValue<Type> deltaSmooth {0.0f};
    Type sampleRate   { Type (44.1e3) };
    Type threshold { Type (0) };
    Type delta { Type (0) };
    Type input_common = { Type (0) };
    Type abs_common{ Type (0) };
    Type abs_common_alpha_beta{ Type (0) };
    Type abs_common_alpha_beta_minus_peak_value{ Type (0) };

    Type abs_common_alpha{ Type (0) };
    Type abs_common_alpha_minus_peak_value{ Type (0) };
    Type abs_common_beta{ Type (0) };
    Type abs_common_alpha_plus_peak_value{ Type (0) };
    Type peak_value_old_beta{ Type (0) };
    Type abs_minus_peak_value_old_beta{ Type (0) };
    Type AlphaBeta { Type (0.6658) };
    Type Alpha { Type (0.00036018) };
    Type Beta { Type (0.00023982) };
   // float Delta = -0.0;
   // float Delta = -0.5;
    //float Delta = -0.001;        // Limiter

    Type peak_value { Type (0) };
    Type peak_value_old { Type (0) };
    Type min_threshold { Type ( 0.0001)};
    Type level_db { Type (0) };
    Type level { Type (0) };
    Type level_old { Type (0) };
   // float threshold  = -30.0;
    Type hysteresis_in { Type (0) };
    Type hysteresis_out { Type (0) };
    Type hysteresis_gamma_delta{ Type (0) };
    Type gain { Type ( 1.0) };
    Type gain_old { Type ( 1.0) };
    Type hub{ Type ( 1.5) };
};
