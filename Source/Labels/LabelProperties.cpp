/*
  ==============================================================================

    LabelProperties.cpp
    Created: 16 Jun 2023 5:32:19pm
    Author:  Wouter Duprez

  ==============================================================================
*/

#include "../PluginEditor.h"

void CompressorV4AudioProcessorEditor::setLabelProperties(juce::Label& label)
{
    addAndMakeVisible(label);
    label.setFont(juce::Font("Helvetica", 16.f, juce::Font::FontStyleFlags::bold));
    label.setJustificationType(juce::Justification::centred);

}