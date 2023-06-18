/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorV4AudioProcessorEditor::CompressorV4AudioProcessorEditor(CompressorV4AudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
	, m_gainDialLabel("Input Gain", "Input Gain")
	, m_threshDiallLabel("Thresh", "Thresh")
	, m_ratioDiallLabel("Ratio", "Ratio")
	, m_attackDialLabel("Attack", "Attack")
	, m_releaseDiallLabel("Release", "Release")
	, m_outputDialLabel("Makeup Gain", "Makeup Gain")
	, m_drywetDialLabel("DryWet", "DryWet")
{
	startTimerHz(42);

	//MeterVisibility
	addAndMakeVisible(m_horizontalMeterL);
	addAndMakeVisible(m_horizontalMeterR);

	//Populating and setting slider properties on all sliders
	for (size_t index = 0; index < m_dials.size(); index++)
	{
		setSliderProperties(*m_dials[index]);
	}

	//Set attachements
	attachSliders();

	//Populating and setting textlabel properties on all sliders
	for (size_t index = 0; index < m_dialLabels.size(); index++)
	{
		setLabelProperties(*m_dialLabels[index]);
		m_dialLabels[index]->attachToComponent(m_dials[index], false);
	}

	//Setting to resizable
	setSize(1000, 500);
	juce::AudioProcessorEditor::setResizable(false, false);
	//juce::AudioProcessorEditor::setResizeLimits(getWidth() * 0.75, getHeight() * 0.75, getWidth() * 1.25, getHeight() * 1.25);
	juce::AudioProcessorEditor::getConstrainer()->setFixedAspectRatio(2.0f);
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
}

CompressorV4AudioProcessorEditor::~CompressorV4AudioProcessorEditor()
{
	for (auto& dial : m_dials)
	{
		dial->setLookAndFeel(nullptr);
	}

	m_dials.clear();
	m_dials.shrink_to_fit();
}

void CompressorV4AudioProcessorEditor::timerCallback()
{
	m_horizontalMeterL.setLevel(audioProcessor.getRMSValue(0));
	m_horizontalMeterR.setLevel(audioProcessor.getRMSValue(1));
	m_horizontalMeterL.repaint();
	m_horizontalMeterR.repaint();
}

//==============================================================================
void CompressorV4AudioProcessorEditor::paint(juce::Graphics& g)
{
	//auto outerUIboxCornerSize = 1 * JUCE_LIVE_CONSTANT(1);
	//auto outerUIboxThickness = 0.1 * JUCE_LIVE_CONSTANT(1);
	//auto outerUIboxWidth = getWidth() * JUCE_LIVE_CONSTANT(0.1);
	//auto outerUIboxHeight = getHeight() * JUCE_LIVE_CONSTANT(0.1);
	// 
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
	
	g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(202, 222, 220).darker(0.75f)
	    ,getHeight()
	    ,juce::Colour::fromRGB(202, 222, 220).brighter(0.02f)
	    ,getHeight() * 0.4));

	//	Main plugin bound background
	g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 20.f);

	g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(202, 222, 220).brighter(0.9f)
		, getHeight()
		, juce::Colour::fromRGB(202, 222, 220).darker(0.3f)
		, getHeight() * 0.4));

	// Rounded bounding box
	g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 20, 19);

	g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(202, 222, 220).brighter(0.2f)
		, getHeight()
		, juce::Colour::fromRGB(202, 222, 220).brighter(0.6f)
		, getHeight() * 0.4));	

	//auto liveConstantDebugMultiplyer = 1 * JUCE_LIVE_CONSTANT(0.1);

	// Meter framing
	g.fillRoundedRectangle(getWidth()* 0.19, 28, 620, 100, 10);

	g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(202, 222, 220).brighter(0.9f)
		, getHeight()
		, juce::Colour::fromRGB(202, 222, 220).darker(0.2f)
		, getHeight() * 0.4));

	// Meter fancy outline
	g.drawRoundedRectangle(190, 28, 620, 100, 10, 2);


	for (size_t index = 0; index < m_dials.size(); index++)
	{
		setSliderProperties(*m_dials[index]);
	}
}

void CompressorV4AudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

		// Calculate dial sizes and margins based on editor dimensions
	auto dialSize = static_cast<int>(getWidth() * 0.13);
	auto mainLeftMargin = static_cast<int>(getWidth() * 0.03);
	auto mainRightMargin = static_cast<int>(getWidth() * 0.03);
	auto verticalMargin = static_cast<int>(getHeight() * 0.3);

	m_attackDial.setBounds(mainLeftMargin
		, 50
		, dialSize
		, dialSize);

	m_releaseDial.setBounds(mainLeftMargin
		, m_attackDial.getY() + verticalMargin,
		dialSize
		, dialSize);

	m_ratioDial.setBounds(mainLeftMargin
		, m_releaseDial.getY() + verticalMargin
		, dialSize
		, dialSize);

	//
	m_threshDial.setBounds(mainLeftMargin*2 + dialSize + mainRightMargin
		, static_cast<int>(getHeight() * 0.32)
		, static_cast<int>(dialSize * 2.4)
		, static_cast<int>(dialSize * 2.4));

	m_outputDial.setBounds(m_threshDial.getX() + m_threshDial.getWidth() + mainRightMargin
		, static_cast<int>(getHeight() * 0.32)
		, static_cast<int>(dialSize * 2.4)
		, static_cast<int>(dialSize * 2.4));
	//

	m_gainDial.setBounds(m_attackDial.getX() + m_attackDial.getWidth() * 6.2
		, 50
		, dialSize
		, dialSize);

	m_drywetDial.setBounds(m_attackDial.getX() + m_attackDial.getWidth() * 6.2
		, m_releaseDial.getY() + verticalMargin
		, dialSize
		, dialSize);

	//auto boxX = 1 * JUCE_LIVE_CONSTANT(0.1);
	//auto boxY = 1 * JUCE_LIVE_CONSTANT(0.1);

	//Meter bounds
	m_horizontalMeterL.setBounds(static_cast<int>(getWidth()* 0.2), static_cast<int>(getWidth() * 0.037), 600, 35);
	m_horizontalMeterR.setBounds(static_cast<int>(getWidth()* 0.2), static_cast<int>(getWidth() * 0.083), 600, 35);
}
