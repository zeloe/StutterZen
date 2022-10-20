/*
  ==============================================================================

    ValueTreeC.h
    Created: 16 Oct 2022 11:18:17pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "DelayView.h"
#include "GuiDefines.h"
class ValueTreeC : public juce::ValueTree::Listener
{
public:
    ValueTreeC(juce::AudioProcessorValueTreeState& apvts,
               DelayView& delayView,
               const juce::String& identifier)
    :ids(identifier), idPoint("point1"),idPoint2("point2"),idPoint3("point3"),idPoint4("point4"),idposx("x"), idposy("y"),
        xval("xval"),yval("yval"),xadd("xadd"),yadd("yadd"),
    valueTree(), delayView(delayView)
    {
        valueTree = apvts.state.getChildWithName(ids);
        if (valueTree.isValid())
        {
           
            {
                auto point1 = valueTree.getChild(0);
                auto x = (float)point1.getProperty(idposx);
                auto y = (float)point1.getProperty(idposy);
                delayView.point1.setPoints(x, y);
                auto xacc = (float)point1.getProperty(xval);
                auto yacc = (float)point1.getProperty(yval);
                delayView.point1.setSpeedXY(xacc,yacc);
                auto xaccadd = (float)point1.getProperty(xadd);
                auto yaccadd = (float)point1.getProperty(yadd);
                delayView.point1.setSpeedXYadd(xaccadd,yaccadd);

                auto point2 = valueTree.getChild(1);
                auto x2 = (float)point2.getProperty(idposx);
                auto y2 = (float)point2.getProperty(idposy);
                delayView.point2.setPoints(x2, y2);
                auto xacc2 = (float)point2.getProperty(xval);
                auto yacc2 = (float)point2.getProperty(yval);
                delayView.point2.setSpeedXY(xacc2,yacc2);
                auto xaccadd2 = (float)point2.getProperty(xadd);
                auto yaccadd2 = (float)point2.getProperty(yadd);
                delayView.point2.setSpeedXYadd(xaccadd2,yaccadd2);
                
                
                
                auto point3 = valueTree.getChild(2);
                auto x3 = (float)point3.getProperty(idposx);
                auto y3 = (float)point3.getProperty(idposy);
                delayView.point3.setPoints(x3, y3);
                auto xacc3 = (float)point3.getProperty(xval);
                auto yacc3 = (float)point3.getProperty(yval);
                delayView.point3.setSpeedXY(xacc3,yacc3);
                auto xaccadd3 = (float)point3.getProperty(xadd);
                auto yaccadd3 = (float)point3.getProperty(yadd);
                delayView.point3.setSpeedXYadd(xaccadd3,yaccadd3);
                
                auto point4=valueTree.getChild(3);
                auto x4 =(float) point4.getProperty(idposx);
                auto y4 = (float)point4.getProperty(idposy);
                delayView.point4.setPoints(x4, y4);
                auto xacc4 = (float)point4.getProperty(xval);
                auto yacc4 = (float)point4.getProperty(yval);
                delayView.point4.setSpeedXY(xacc4,yacc4);
                auto xaccadd4 = (float)point2.getProperty(xadd);
                auto yaccadd4 = (float)point2.getProperty(yadd);
                delayView.point4.setSpeedXYadd(xaccadd4,yaccadd4);
                
              //  for(auto& p : dlview)
             //       p.points.setPoints(x,y);
            }
            DBG("/////////////////////////////");
            DBG(valueTree.toXmlString());
        } else
        {
            valueTree = juce::ValueTree(ids);
            apvts.state.appendChild(valueTree,nullptr);
            delayView.point1.setPoints(width1/2,height1/2);
            delayView.point2.setPoints(width1/2,height1/2);
            delayView.point3.setPoints(width1/2,height1/2);
            delayView.point4.setPoints(width1/2,height1/2);
            delayView.point1.setSpeedStart();
            delayView.point2.setSpeedStart();
            delayView.point3.setSpeedStart();
            delayView.point4.setSpeedStart();
        }
        valueTree.addListener(this);
    }
    
    ~ValueTreeC()
    {
        valueTree.removeListener(this);
    };
    void valueTreePropertyChanged ()
    {
        valueTree.removeAllChildren(nullptr);
        juce::ValueTree pointsingle1(idPoint);
       
        pointsingle1.setProperty(idposx, delayView.point1.getPosX(),nullptr);
        pointsingle1.setProperty(idposy, delayView.point1.getPosY(),nullptr);
        pointsingle1.setProperty(xval, delayView.point1.getXSpeed(),nullptr);
        pointsingle1.setProperty(yval, delayView.point1.getYSpeed(),nullptr);
        pointsingle1.setProperty(xadd, delayView.point1.getXSpeedadd(),nullptr);
        pointsingle1.setProperty(yadd, delayView.point1.getYSpeedadd(),nullptr);
        juce::ValueTree pointsingle2(idPoint2);
       
        pointsingle2.setProperty(idposx, delayView.point2.getPosX(),nullptr);
        pointsingle2.setProperty(idposy, delayView.point2.getPosY(),nullptr);
        pointsingle2.setProperty(xval, delayView.point2.getXSpeed(),nullptr);
        pointsingle2.setProperty(yval, delayView.point2.getYSpeed(),nullptr);
        pointsingle2.setProperty(xadd, delayView.point2.getXSpeedadd(),nullptr);
        pointsingle2.setProperty(yadd, delayView.point2.getYSpeedadd(),nullptr);
        juce::ValueTree pointsingle3(idPoint3);
        
        pointsingle3.setProperty(idposx, delayView.point3.getPosX(),nullptr);
        pointsingle3.setProperty(idposy, delayView.point3.getPosY(),nullptr);
        pointsingle3.setProperty(xval, delayView.point3.getXSpeed(),nullptr);
        pointsingle3.setProperty(yval, delayView.point3.getYSpeed(),nullptr);
        pointsingle3.setProperty(xadd, delayView.point3.getXSpeedadd(),nullptr);
        pointsingle3.setProperty(yadd, delayView.point3.getYSpeedadd(),nullptr);
        juce::ValueTree pointsingle4(idPoint4);
       
        pointsingle4.setProperty(idposx, delayView.point4.getPosX(),nullptr);
        pointsingle4.setProperty(idposy, delayView.point4.getPosY(),nullptr);
        pointsingle4.setProperty(xval, delayView.point4.getXSpeed(),nullptr);
        pointsingle4.setProperty(yval, delayView.point4.getYSpeed(),nullptr);
        pointsingle4.setProperty(xadd, delayView.point4.getXSpeedadd(),nullptr);
        pointsingle4.setProperty(yadd, delayView.point4.getYSpeedadd(),nullptr);
        
        
        valueTree.appendChild(pointsingle1,nullptr);
        valueTree.appendChild(pointsingle2,nullptr);
        valueTree.appendChild(pointsingle3,nullptr);
        valueTree.appendChild(pointsingle4,nullptr);
        DBG(valueTree.toXmlString());
    }
    
    
    juce::Identifier ids,idPoint, idPoint2,idPoint3,idPoint4, idposx,idposy;
    juce::Identifier xval, yval, xadd,yadd;
    juce::ValueTree valueTree;
    DelayView& delayView;
private:
    
};
