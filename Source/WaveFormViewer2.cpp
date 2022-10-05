/*
  ==============================================================================

    WaveFormViewer.cpp
    Created: 4 Oct 2022 4:16:52pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#include "WaveFormViewer2.h"


struct WaveFromViewer_2::ChannelInfo
{
    ChannelInfo (WaveFromViewer_2& o, int bufferSize) : owner (o)
    {
        setBufferSize (bufferSize);
        clear();
    }

    void clear() noexcept
    {
        levels.fill ({});
        value = {};
        subSample = 0;
    }

    void pushSamples (const float* inputSamples, int num) noexcept
    {
        for (int i = 0; i < num; ++i)
            pushSample (inputSamples[i]);
    }

    void pushSample (float newSample) noexcept
    {
        if (--subSample <= 0)
        {
            if (++nextSample == levels.size())
                nextSample = 0;

            levels.getReference (nextSample) = value;
            subSample = owner.getSamplesPerBlock();
            value = juce::Range<float> (newSample, newSample);
        }
        else
        {
            value = value.getUnionWith (newSample);
        }
    }

    void setBufferSize (int newSize)
    {
        levels.removeRange (newSize, levels.size());
        levels.insertMultiple (-1, {}, newSize - levels.size());

        if (nextSample >= newSize)
            nextSample = 0;
    }

    WaveFromViewer_2& owner;
    juce::Array<juce::Range<float>> levels;
    juce::Range<float> value;
    std::atomic<int> nextSample { 0 }, subSample { 0 };

};

//==============================================================================
WaveFromViewer_2::WaveFromViewer_2 (int initialNumChannels)
  : numSamples (1024),
    inputSamplesPerBlock (256),
    backgroundColour (juce::Colours::black),
    waveformColour (juce::Colours::white)
{
    setOpaque (true);
    setNumChannels (initialNumChannels);
    setRepaintRate (60);
}

WaveFromViewer_2::~WaveFromViewer_2()
{
}

void WaveFromViewer_2::setNumChannels (int numChannels)
{
    channels.clear();

    for (int i = 0; i < numChannels; ++i)
        channels.add (new ChannelInfo (*this, numSamples));
}

void WaveFromViewer_2::setBufferSize (int newNumSamples)
{
    numSamples = newNumSamples;

    for (auto* c : channels)
        c->setBufferSize (newNumSamples);
}

void WaveFromViewer_2::clear()
{
    for (auto* c : channels)
        c->clear();
}

void WaveFromViewer_2::pushBuffer (const float** d, int numChannels, int num)
{
    numChannels = juce::jmin (numChannels, channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSamples (d[i], num);
}

void WaveFromViewer_2::pushBuffer (const juce::AudioBuffer<float>& buffer)
{
    pushBuffer (buffer.getArrayOfReadPointers(),
                buffer.getNumChannels(),
                buffer.getNumSamples());
}

void WaveFromViewer_2::pushBuffer (const juce::AudioSourceChannelInfo& buffer)
{
    auto numChannels = juce::jmin (buffer.buffer->getNumChannels(), channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSamples (buffer.buffer->getReadPointer (i, buffer.startSample),
                                               buffer.numSamples);
}

void WaveFromViewer_2::pushSample (const float* d, int numChannels)
{
    numChannels = juce::jmin (numChannels, channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSample (d[i]);
}

void WaveFromViewer_2::setSamplesPerBlock (int newSamplesPerPixel) noexcept
{
    inputSamplesPerBlock = newSamplesPerPixel;
}

void WaveFromViewer_2::setRepaintRate (int frequencyInHz)
{
    startTimerHz (frequencyInHz);
}

void WaveFromViewer_2::timerCallback()
{
    repaint();
}

void WaveFromViewer_2::setColours (juce::Colour bk, juce::Colour fg) noexcept
{
    backgroundColour = bk;
    waveformColour = fg;
    repaint();
}

void WaveFromViewer_2::paint (juce::Graphics& g)
{
    g.fillAll (backgroundColour);

    auto r = getLocalBounds().toFloat();
    auto channelHeight = r.getHeight() / (float) channels.size();

    g.setColour (waveformColour);

    for (auto* c : channels)
    {
        if(counter == 0)
        {
            g.setColour(juce::Colours::red);
        } else if(counter == 1)
        {
            g.setColour(juce::Colours::green);
        }
        
        counter++;
        if (counter > 1)
            counter = 0;
        paintChannel (g, r.removeFromTop (channelHeight),
                      c->levels.begin(), c->levels.size(), c->nextSample);
    }
}

void WaveFromViewer_2::getChannelAsPath (juce::Path& path, const juce::Range<float>* levels,
                                                 int numLevels, int nextSample)
{
    path.preallocateSpace (4 * numLevels + 8);

    for (int i = 0; i < numLevels; ++i)
    {
        auto level = -(levels[(nextSample + i) % numLevels].getEnd());

        if (i == 0)
            path.startNewSubPath (0.0f, level);
        else
            path.lineTo ((float) i, level);
    }

    for (int i = numLevels; --i >= 0;)
        path.lineTo ((float) i, -(levels[(nextSample + i) % numLevels].getStart()));

    path.closeSubPath();
}

void WaveFromViewer_2::paintChannel (juce::Graphics& g, juce::Rectangle<float> area,
                                             const juce::Range<float>* levels, int numLevels, int nextSample)
{
    juce::Path p;
  
    getChannelAsPath (p, levels, numLevels, nextSample);
    
    
    g.fillPath (p, juce::AffineTransform::fromTargetPoints (0.0f, -1.0f,               area.getX(), area.getY(),
                                                      0.0f, 1.0f,                area.getX(), area.getBottom(),
                                                      (float) numLevels, -1.0f,  area.getRight(), area.getY()));
    
    
}


