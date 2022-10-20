/*
  ==============================================================================

    DelayView.h
    Created: 13 Oct 2022 3:50:27pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "GuiDefines.h"
#include "Points.h"
class DelayView: public juce::AnimatedAppComponent
{
public:
//iterate trough arrays in constructor
    DelayView
    (
     juce::Colour colour1 = juce::Colours::blue,
     juce::Colour colour2 = juce::Colours::green,
     juce::Colour colour3 = juce::Colours::yellow,
     juce::Colour colour4 = juce::Colours::red
     ):
    point1(colour1),
    point2(colour2),
    point3(colour3),
    point4(colour4)
   
    
    {
        setFramesPerSecond (30);
    }
    ~DelayView()
    {}
    void update() override
    {
        point1.update();
        point2.update();
        point3.update();
        point4.update();
    }/** Your subclass can call this to start a timer running which will
        call update() and repaint the component at the given frequency.
    */
    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::white);
        g.drawRect(0,0, width1, height1);
        point1.paint(g);
        point2.paint(g);
        point3.paint(g);
        point4.paint(g);
    }
    Points point1, point2,point3,point4;
    int start = 0;
    
private:
    
};
