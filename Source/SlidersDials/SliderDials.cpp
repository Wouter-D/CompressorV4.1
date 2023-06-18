/*
  ==============================================================================

	SliderDials.cpp
	Created: 16 Jun 2023 4:23:42pm
	Author:  Wouter Duprez

  ==============================================================================
*/

//Courtesy of the AudioPluginCodersGuild

#include "../PluginEditor.h"

void CompressorV4AudioProcessorEditor::setSliderProperties(juce::Slider& slider)
{
	addAndMakeVisible(slider);

	slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);

	//  Text stuff
	slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 30);
	slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);

	//  Manual setting for greater/ easier to read style setting
	addAndMakeVisible(m_gainDial);
	m_gainDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	m_gainDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 30);
	//m_gainDial.setRange(-60.f, 24.f, 0.01f);
	m_gainDial.setLookAndFeel(&customDialLookAndFeel);
	m_gainDial.setTextValueSuffix("dB");

	addAndMakeVisible(m_threshDial);
	m_threshDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	m_threshDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 30);
	//m_threshDial.setRange(-60.f, 24.f, 0.01f);
	m_threshDial.setLookAndFeel(&customDialLookAndFeel);
	m_threshDial.setTextValueSuffix("dB");

	addAndMakeVisible(m_ratioDial);
	m_ratioDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	m_ratioDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 30);
	//m_ratioDial.setRange(1.f, 20.0f, 1.0f);
	m_ratioDial.setLookAndFeel(&customDialLookAndFeel);

	addAndMakeVisible(m_attackDial);
	m_attackDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	m_attackDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 30);
	//m_attackDial.setRange(0.f, 200.f, 1.f);
	m_attackDial.setLookAndFeel(&customDialLookAndFeel);

	addAndMakeVisible(m_releaseDial);
	m_releaseDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	m_releaseDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 30);
	//m_releaseDial.setRange(5.f, 5000.f, 1.f);
	m_releaseDial.setLookAndFeel(&customDialLookAndFeel);

	addAndMakeVisible(m_outputDial);
	m_outputDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	m_outputDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 30);
	//m_outputDial.setRange(-60.f, 24.f, 1.f);
	m_outputDial.setLookAndFeel(&customDialLookAndFeel);
	m_outputDial.setTextValueSuffix("dB");

	addAndMakeVisible(m_drywetDial);
	m_drywetDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	m_drywetDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 30);
	//m_drywetDial.setRange(0.f, 1.f, 0.1f);
	m_drywetDial.setLookAndFeel(&customDialLookAndFeel);
	m_drywetDial.setTextValueSuffix("%"); addAndMakeVisible(m_gainDial);

	m_gainDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	m_gainDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 30);
	//m_gainDial.setRange(-60.f, 24.f, 0.01f);
	m_gainDial.setLookAndFeel(&customDialLookAndFeel);
	m_gainDial.setTextValueSuffix("dB");
}

void CompressorV4AudioProcessorEditor::setLabelProperties(juce::Label& label)
{

}

void CompressorV4AudioProcessorEditor::attachSliders()
{
	m_gainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.m_treeState, m_inputgainID, m_gainDial);

	m_threshAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.m_treeState, m_threshID, m_threshDial);

	m_ratioAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.m_treeState, m_ratioID, m_ratioDial);

	m_attackAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.m_treeState, m_attackID, m_attackDial);

	m_releaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.m_treeState, m_releaseID, m_releaseDial);

	m_outputAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.m_treeState, m_makeupgainID, m_outputDial);

	m_drywetAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.m_treeState, m_wetdryID, m_drywetDial);
}
