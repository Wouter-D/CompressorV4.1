/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters/Parameters.h"
#include "LevelMeter/LevelMeter.h"
#include "LevelMeter/Fifo.h"

//==============================================================================
/**
*/
class CompressorV4AudioProcessor : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    CompressorV4AudioProcessor(); // Constructor
    ~CompressorV4AudioProcessor() override; // Destructor

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override; // Prepare the processor to play audio
    void releaseResources() override; // Release any resources used by the processor

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override; // Check if the specified bus layout is supported
#endif
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override; // Process audio in the block
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override; // Create the editor for the processor
    bool hasEditor() const override; // Check if the processor has an editor
    //==============================================================================
    const juce::String getName() const override; // Get the name of the processor
    bool acceptsMidi() const override; // Check if the processor accepts MIDI input
    bool producesMidi() const override; // Check if the processor produces MIDI output
    bool isMidiEffect() const override; // Check if the processor is a MIDI effect
    double getTailLengthSeconds() const override; // Get the tail length in seconds
    //==============================================================================
    int getNumPrograms() override; // Get the number of programs
    int getCurrentProgram() override; // Get the index of the current program
    void setCurrentProgram(int index) override; // Set the index of the current program
    const juce::String getProgramName(int index) override; // Get the name of a program
    void changeProgramName(int index, const juce::String& newName) override; // Change the name of a program

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override; // Get the state information of the processor
    void setStateInformation(const void* data, int sizeInBytes) override; // Set the state information of the processor

    juce::AudioProcessorValueTreeState m_treeState; // Value tree state for managing parameters
    float getRMSValue(const int channel) const;

private:
    juce::dsp::ProcessSpec m_spec; // Processing specifications
    juce::dsp::Gain<float> m_inputModule; // Input gain module
    juce::dsp::Gain<float> m_outputModule; // Output gain module
    juce::dsp::Compressor<float> m_compressorModule; // Compressor module
    juce::dsp::DryWetMixer<float> m_drywetModule;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout(); // Create the parameter layout
    void parameterChanged(const juce::String& parameterID, float newValue) override; // Called when a parameter value changes
    void updateParameters(); // Update the parameters of the processor

    float m_rmsvalueLeft, m_rmsvalueRight;
    float m_wetDryMixValue;

    //float m_blendValue = 0.5f;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorV4AudioProcessor)
};
