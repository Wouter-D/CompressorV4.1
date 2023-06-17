/*
  ==============================================================================

    LevelMeter.cpp
    Created: 17 Jun 2023 5:36:50pm
    Author:  Wouter Duprez

  ==============================================================================
*/

#include "LevelMeter.h"

void Gui::LevelMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    //g.setColour(juce::Colours::red.withBrightness(0.1f));
   // g.setGradientFill(juce::ColourGradient::vertical(juce::Colours::midnightblue.darker(0.05f), juce::Colours::cadetblue.brighter(0.05f),getLocalBounds()));
    g.setGradientFill(juce::ColourGradient::horizontal(juce::Colours::midnightblue.darker(0.05f), juce::Colours::cadetblue.brighter(0.05f),getLocalBounds()));

    g.fillRoundedRectangle(bounds, 5.f);

    g.setColour(juce::Colours::white);
    //-60 to +6
    const auto scaledX = juce::jmap(m_level, -60.f, +6.f, 0.f, static_cast<float>(getWidth()));
    g.fillRoundedRectangle(bounds.removeFromLeft(scaledX), 5.f);
}

void Gui::LevelMeter::setLevel(const float value)
{
    m_level = value;
}
