/*
  ==============================================================================

    DialLookAndFeel.h
    Created: 16 Jun 2023 7:22:40pm
    Author:  Wouter Duprez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DialLookAndFeel : public juce::LookAndFeel_V4
{
public:
    DialLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;

    void drawLabel(juce::Graphics& g, juce::Label& label) override;
    void getBaseFont();
   
private:
    float m_sliderWidth;
};