/*
  ==============================================================================

    LevelMeter.h
    Created: 17 Jun 2023 5:36:50pm
    Author:  Wouter Duprez

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Gui
{
    class LevelMeter : public juce::Component
    {
        public:
            void paint(juce::Graphics& g)override;
            void setLevel(const float value);

        private:
            float m_level = -60.f;

    };
}