/*
  ==============================================================================

    Points.h
    Created: 13 Oct 2022 5:46:38pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "GuiDefines.h"
class Points:
public juce::Component
{
public:
    Points
    (juce::Colour colour)
     {
         colour1 = colour;

    }
     
    
    ~Points(){};
   // Points(const Points&) = delete;
   // Points& operator=(const Points&) = delete;
    void update()
    {
        xpos = xpos + ((sxspeed + xspeed) * xdirection );
        ypos = ypos + ((syspeed +yspeed) * ydirection );
          
          // Test to see if the shape exceeds the boundaries of the screen
          // If it does, reverse its direction by multiplying by -1
        if (xpos > width1-sizeoff || xpos < sizeoff)
        {
            xdirection *= -1;
        }
          if (ypos > height1-sizeoff || ypos < sizeoff)
        {
            ydirection *= -1;
        }
    }
    
    const float getPosX ()
    {
        return xpos;
    }
    const float getPosY ()
    {
        return ypos;
    }
    void setPointColor(juce::Colour colours)
    {
        colour1 =colours;
    }
    
    void changeSpeed()
    {
        xspeed = rand1.nextFloat() + sxspeed;
        yspeed = rand1y.nextFloat() +syspeed;
    }
    
    void setSpeedStart()
    {
        xspeed = rand1.nextFloat() + sxspeed;
        yspeed = rand1y.nextFloat() +syspeed;
    }
    
    void scaleSpeed(float speed)
    {
        sxspeed = speed;
        syspeed = speed;
        
    }
    
    
    void paint (juce::Graphics& g) override
    {
        g.setColour(colour1);
        g.fillEllipse(xpos,ypos,size,size);
    }
    
    float getXSpeed()
    {
        return xspeed;
    }
    
    float getYSpeed()
    {
        return yspeed;
    }
    
    float getXSpeedadd()
    {
        return sxspeed;
    }
    
    float getYSpeedadd()
    {
        return syspeed;
    }
    
    void reset()
    {
        xspeed = 0;
        yspeed = 0;
        sxspeed = 0;
        syspeed = 0;
    }
    
    void resetPos()
    {
        xpos = width1 /2;
        ypos = width2 /2;
    }
    
    void setPoints(float x, float y)
    {
        xpos = x;
        ypos = y;
    }
    
    void setSpeedXY(float x, float y)
    {
        xspeed = x;
        yspeed = y;
    }
    
    void setSpeedXYadd(float x, float y)
    {
        sxspeed = x;
        syspeed = y;
    }
   
    juce::Colour colour1;
private:
    float xpos ;
    float ypos ;
    juce::Random rand1, rand1y;
    int size = 10;
    float xspeed;  // Speed of the shape
    float yspeed;  // Speed of the shape
    float sxspeed;  // Speed of the shape
    float syspeed;  // Speed of the shape
    float pxspeed;  // Speed of the shape
    float pyspeed;  // Speed of the shape
    float psxspeed;  // Speed of the shape
    float psyspeed;  // Speed of the shape
    int sizeoff = 20;
    int xdirection = 1;  // Left or Right
    int ydirection = 1;  // Top to Bottom
protected:
       
       
        
};
