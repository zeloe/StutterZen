/*
  ==============================================================================

    WaveFormViewer.cpp
    Created: 4 Oct 2022 4:16:52pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#include "WaveFormViewer.h"


struct WaveFromViewer::ChannelInfo
{
    ChannelInfo (WaveFromViewer& o, int bufferSize) : owner (o)
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

    WaveFromViewer& owner;
    juce::Array<juce::Range<float>> levels;
    juce::Range<float> value;
    std::atomic<int> nextSample { 0 }, subSample { 0 };

};

//==============================================================================
WaveFromViewer::WaveFromViewer (int initialNumChannels)
  : numSamples (1024),
    inputSamplesPerBlock (256),
    backgroundColour (juce::Colours::black),
    waveformColour (juce::Colours::white)
{
    setOpaque (true);
    setNumChannels (initialNumChannels);
    setRepaintRate (60);
}

WaveFromViewer::~WaveFromViewer()
{
}

void WaveFromViewer::setNumChannels (int numChannels)
{
    channels.clear();

    for (int i = 0; i < numChannels; ++i)
        channels.add (new ChannelInfo (*this, numSamples));
}

void WaveFromViewer::setBufferSize (int newNumSamples)
{
    numSamples = newNumSamples;

    for (auto* c : channels)
        c->setBufferSize (newNumSamples);
}

void WaveFromViewer::clear()
{
    for (auto* c : channels)
        c->clear();
}

void WaveFromViewer::pushBuffer (const float** d, int numChannels, int num)
{
    numChannels = juce::jmin (numChannels, channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSamples (d[i], num);
}

void WaveFromViewer::pushBuffer (const juce::AudioBuffer<float>& buffer)
{
    pushBuffer (buffer.getArrayOfReadPointers(),
                buffer.getNumChannels(),
                buffer.getNumSamples());
}

void WaveFromViewer::pushBuffer (const juce::AudioSourceChannelInfo& buffer)
{
    auto numChannels = juce::jmin (buffer.buffer->getNumChannels(), channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSamples (buffer.buffer->getReadPointer (i, buffer.startSample),
                                               buffer.numSamples);
}

void WaveFromViewer::pushSample (const float* d, int numChannels)
{
    numChannels = juce::jmin (numChannels, channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSample (d[i]);
}

void WaveFromViewer::setSamplesPerBlock (int newSamplesPerPixel) noexcept
{
    inputSamplesPerBlock = newSamplesPerPixel;
}

void WaveFromViewer::setRepaintRate (int frequencyInHz)
{
    startTimerHz (frequencyInHz);
}

void WaveFromViewer::timerCallback()
{
    repaint();
}

void WaveFromViewer::setColours (juce::Colour bk, juce::Colour fg) noexcept
{
    backgroundColour = bk;
    waveformColour = fg;
    repaint();
}

void WaveFromViewer::paint (juce::Graphics& g)
{
    g.fillAll (backgroundColour);

    auto r = getLocalBounds().toFloat();
    auto channelHeight = r.getHeight() / (float) channels.size();

    g.setColour (waveformColour);

    for (auto* c : channels)
        paintChannel (g, r.removeFromTop (channelHeight),
                      c->levels.begin(), c->levels.size(), c->nextSample);
}

void WaveFromViewer::getChannelAsPath (juce::Path& path, const juce::Range<float>* levels,
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

void WaveFromViewer::paintChannel (juce::Graphics& g, juce::Rectangle<float> area,
                                             const juce::Range<float>* levels, int numLevels, int nextSample)
{
    juce::Path p;
    getChannelAsPath (p, levels, numLevels, nextSample);
    g.setColour(juce::Colours::white);
    g.fillPath (p, juce::AffineTransform::fromTargetPoints (0.0f, -1.0f,               area.getX(), area.getY(),
                                                      0.0f, 1.0f,                area.getX(), area.getBottom(),
                                                      (float) numLevels, -1.0f,  area.getRight(), area.getY()));
    g.setColour(juce::Colours::red);
    g.drawLine(area.getX() * 4, LineHeightL / 120.f * 50, area.getY() * 8, LineHeightL / 120.f * 50);
    g.setColour(juce::Colours::green);
    g.drawLine(area.getX() * 4, LineHeightR / 120.f * 50 +50,  area.getY() * 8, LineHeightR / 120.f * 50 + 50);
   // g.drawLine(<#float startX#>, <#float startY#>, <#float endX#>, <#float endY#>)
}


void WaveFromViewer::updateLineL (int newValue)
{
    LineHeightL = newValue;
}
void WaveFromViewer::updateLineR (int newValue)
{
    LineHeightR = newValue;
}
