/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel/DialLookAndFeel.h"
#include "LevelMeter/LevelMeter.h"


//==============================================================================
/**
*/

class CompressorV4AudioProcessorEditor  : public juce::AudioProcessorEditor
    , public juce::Timer
{
public:
    CompressorV4AudioProcessorEditor (CompressorV4AudioProcessor&);
    ~CompressorV4AudioProcessorEditor() override;

    //==============================================================================
    void timerCallback() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CompressorV4AudioProcessor& audioProcessor; // Reference to the CompressorV4AudioProcessor object

    // Sliders
    juce::Slider m_gainDial; // Slider for input
    juce::Slider m_threshDial; // Slider for threshold
    juce::Slider m_ratioDial; // Slider for ratio
    juce::Slider m_attackDial; // Slider for attack
    juce::Slider m_releaseDial; // Slider for release
    juce::Slider m_outputDial; // Slider for output
    juce::Slider m_drywetDial; // Slider for output

    // LookAndFeel
    DialLookAndFeel customDialLookAndFeel; 
    
    // Vector of pointers to the sliders
    std::vector<juce::Slider*> m_dials =
    {
        &m_gainDial,
        &m_threshDial,
        &m_ratioDial,
        &m_attackDial,
        &m_releaseDial,
        &m_outputDial,
        & m_drywetDial
    }; 

    // Labels
    juce::Label m_gainDialLabel; 
    juce::Label m_threshDiallLabel;
    juce::Label m_ratioDiallLabel; 
    juce::Label m_attackDialLabel; 
    juce::Label m_releaseDiallLabel;
    juce::Label m_outputDialLabel; 
    juce::Label m_drywetDialLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_gainAttach; // Attachment for input slider
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_threshAttach; // Attachment for threshold slider
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_ratioAttach; // Attachment for ratio slider
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_attackAttach; // Attachment for attack slider
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_releaseAttach; // Attachment for release slider
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_outputAttach; // Attachment for output slider
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_drywetAttach; // Attachment for output slider
    

    std::vector<juce::Label*> m_dialLabels =
    {
        &m_gainDialLabel,
        &m_threshDiallLabel,
        &m_ratioDiallLabel,
        &m_attackDialLabel,
        &m_releaseDiallLabel,
        &m_outputDialLabel,
        &m_drywetDialLabel
    }; // Vector of pointers to the labels for the dials

    void attachSliders(); // Function to attach sliders to AudioProcessorValueTreeState
    void setSliderProperties(juce::Slider& slider); // Function to set properties of a slider
    void setLabelProperties(juce::Label& label); // Function to set properties of a label

    Gui::LevelMeter m_horizontalMeterL, m_horizontalMeterR;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorV4AudioProcessorEditor)
};
