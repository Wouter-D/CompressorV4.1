
/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

//  This code was based on tutorials and documentation of:
//  The Audio Programmer - Juce Tutorial series
//  DrBruisin - Compressor Series
//  freeCodeCamp - MatKatMusic's EQ series
//  ChatGPT

//  Roadmap:
//
//  1. Implement needed parameters
//      -What dials do I need?
//      -What modules?
//      -How to update parameter values?
//
//  2. Build basic compressor
//      -Compression calculation
//      -Set parameter ranges
//      -Set parameter values to ranges
//
//  3. Build basic GUI
//      -Implement correct dial placement
//      -Apply basic concept color pallet
//
//  4. Attach parameters to GUI dials
//
//  5. Duplicate audio buffer to mix wet and dry
//      -Duplicate inputSignal to dry and wet
//      -Implement dry/wet mix
//
//  6. Build metering
//      -RMS check?
//      -Implement visual metering
//      -Attach rms return to meter
//
//  7. Customize GUI
//      -Set a custom look and feel based on initial concept
//
//  8. Finetune
//
//  9.Ship

//  Used ChatGPT as virtual rubberducky. Learned about the juce framework by asking what each block provided does.
//  Implemented and iterated modules and parameters based in the DrBruisin compressor concept
//  Constant refactoring using ChatGPT. Asking it literally if my code made sense and how to best approach correct implementation.
//
//  Due to Git authentication, pushing to repo and partial file corruptions, I had to re-do the remote push a few times.
//  While annoying to re-visit code in a panicked state isn't fun. It did force me to be more efficient handling projects.

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorV4AudioProcessor::CompressorV4AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
	, m_treeState(*this, nullptr, "PARAMETERS", createParameterLayout())

#endif
{
	//m_treeState.addParameterListener(m_inputgainID, this);
	//m_treeState.addParameterListener(m_threshID, this);
	//m_treeState.addParameterListener(m_ratioID, this);
	//m_treeState.addParameterListener(m_attackID, this);
	//m_treeState.addParameterListener(m_releaseID, this);
	//m_treeState.addParameterListener(m_makeupgainID, this);

	const juce::StringArray parameterIDs = { m_inputgainID, m_threshID, m_ratioID, m_attackID, m_releaseID, m_makeupgainID,m_wetdryID };
	for (const juce::String& parameterID : parameterIDs)
	{
		m_treeState.addParameterListener(parameterID, this);
	}
}

CompressorV4AudioProcessor::~CompressorV4AudioProcessor()
{
	//m_treeState.removeParameterListener(m_inputgainID, this);
	//m_treeState.removeParameterListener(m_threshID, this);
	//m_treeState.removeParameterListener(m_ratioID, this);
	//m_treeState.removeParameterListener(m_attackID, this);
	//m_treeState.removeParameterListener(m_releaseID, this);
	//m_treeState.removeParameterListener(m_makeupgainID, this);

	// Remove parameter listeners
	const juce::StringArray parameterIDs = { m_inputgainID, m_threshID, m_ratioID, m_attackID, m_releaseID, m_makeupgainID, m_wetdryID };
	for (const juce::String& parameterID : parameterIDs)
	{
		m_treeState.removeParameterListener(parameterID, this);
	}

}

void CompressorV4AudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
	updateParameters();
}

juce::AudioProcessorValueTreeState::ParameterLayout CompressorV4AudioProcessor::createParameterLayout()
{
	//  Use smart pointers in ParameterLayout creation:
	//  Instead of manually creating unique pointers and pushing them into the params vector,
	//  you can use std::make_unique directly in the push_back function

	std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

	juce::NormalisableRange<float> attackRange = juce::NormalisableRange<float>(0.01f, 30.0f, 1.0f);
	attackRange.setSkewForCentre(25.0f);

	juce::NormalisableRange<float> releaseRange = juce::NormalisableRange<float>(0.1f, 5000.0f, 1.0f);
	releaseRange.setSkewForCentre(160.0f);

	params.push_back(std::make_unique<juce::AudioParameterFloat>(m_inputgainID, m_makeupgainName, -60.0f, 0.0f, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(m_threshID, m_threshName, -40.0f, 0.0f, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(m_ratioID, m_ratioName, 2.0f, 20.0f, 1.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(m_attackID, m_attackName, attackRange, 0.01f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(m_releaseID, m_releaseName, releaseRange, 6.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(m_makeupgainID, m_makeupgainName, 0.0f, 20.0f, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(m_wetdryID, m_wetdryID, 0, 1, 1.0f));


	return { params.begin(), params.end() };


	//Old manual version =

   /* auto pInput = std::make_unique<juce::AudioParameterFloat>(m_inputgainID,
		m_makeupgainName,
		-60.0f,
		24.0f,
		0.0f);

	auto pThresh = std::make_unique<juce::AudioParameterFloat>(m_threshID,
		m_threshName,
		-60.0f,
		10.0f,
		0.0f);

	auto pRatio = std::make_unique<juce::AudioParameterFloat>(m_ratioID,
		m_ratioName,
		1.0f,
		20.0f,
		1.0f);

	auto pAttack = std::make_unique<juce::AudioParameterFloat>(m_attackID,
		m_attackName,
		attackRange,
		50.0f);

	auto pRelease = std::make_unique<juce::AudioParameterFloat>(m_releaseID,
		m_releaseName,
		releaseRange,
		160.0f);

	auto pOutput = std::make_unique<juce::AudioParameterFloat>(m_makeupgainID,
		m_makeupgainName,
		-60.0f,
		24.0f,
		0.0f);

	params.push_back(std::move(pInput));
	params.push_back(std::move(pThresh));
	params.push_back(std::move(pRatio));
	params.push_back(std::move(pAttack));
	params.push_back(std::move(pRelease));
	params.push_back(std::move(pOutput));

	return { params.begin(), params.end() };*/
}

void CompressorV4AudioProcessor::updateParameters()
{
	//Update all DSP module parameters
	m_inputModule.setGainDecibels(m_treeState.getRawParameterValue(m_inputgainID)->load());
	m_compressorModule.setThreshold(m_treeState.getRawParameterValue(m_threshID)->load());
	m_compressorModule.setRatio(m_treeState.getRawParameterValue(m_ratioID)->load());
	m_compressorModule.setAttack(m_treeState.getRawParameterValue(m_attackID)->load());
	m_compressorModule.setRelease(m_treeState.getRawParameterValue(m_releaseID)->load());
	m_outputModule.setGainDecibels(m_treeState.getRawParameterValue(m_makeupgainID)->load());

	//WETDRY
	m_wetDryMixValue = m_treeState.getRawParameterValue(m_wetdryID)->load();
}

//==============================================================================
const juce::String CompressorV4AudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool CompressorV4AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool CompressorV4AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool CompressorV4AudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double CompressorV4AudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int CompressorV4AudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int CompressorV4AudioProcessor::getCurrentProgram()
{
	return 0;
}

void CompressorV4AudioProcessor::setCurrentProgram(int index)
{
}

const juce::String CompressorV4AudioProcessor::getProgramName(int index)
{
	return {};
}

void CompressorV4AudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================

void CompressorV4AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	m_rmsvalueLeft.reset(sampleRate,0.2);
	m_rmsvalueRight.reset(sampleRate, 0.2);

	m_rmsvalueLeft.setCurrentAndTargetValue(-100.f);
	m_rmsvalueRight.setCurrentAndTargetValue(-100.f);

	// Initialize spec for dsp modules
	m_spec.maximumBlockSize = samplesPerBlock;
	m_spec.sampleRate = sampleRate;
	m_spec.numChannels = getTotalNumOutputChannels();

	// Prepare DSP modules for processing
	m_inputModule.prepare(m_spec);
	m_outputModule.prepare(m_spec);
	m_compressorModule.prepare(m_spec);

	//Smooth change from parameter
	m_inputModule.setRampDurationSeconds(0.05);
	m_outputModule.setRampDurationSeconds(0.05);

	updateParameters();
}

void CompressorV4AudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorV4AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void CompressorV4AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	updateParameters();

	juce::ScopedNoDenormals noDenormals;

	//Duplicating the audio block for wet dry mixing
	//Unsure if this will cause phase issue
	juce::dsp::AudioBlock<float> block{ buffer };
	juce::AudioBuffer<float> dryBuffer;
	dryBuffer.makeCopyOf(buffer);

	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	//buffer.getRMSLevel(0, 0, buffer.getNumSamples());
	//buffer.getRMSLevel(1, 0, buffer.getNumSamples());

	//Thanks to Baue =
	dryBuffer.makeCopyOf(buffer);

	//Process DSP modules
	m_inputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
	m_compressorModule.process(juce::dsp::ProcessContextReplacing<float>(block));
	m_outputModule.process(juce::dsp::ProcessContextReplacing<float>(block));

	// Apply wet/dry mix
	const float wetLevel = m_wetDryMixValue;  // Adjust this value to control the wet level (0.0 to 1.0)
	const float dryLevel = 1.0f - m_wetDryMixValue;
	// Iterate over each input channel
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		auto* channelDataDry = dryBuffer.getWritePointer(channel);
		const auto* inputData = buffer.getReadPointer(channel);

		//Dry wet algo concept by Kaatje and Baeue
		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			// Iterate over each sample in the buffer
			if (m_wetDryMixValue == 0)
			{
				// If wet/dry mix is 0, only dry signal is used
				channelData[sample] = channelDataDry[sample];
			}

			else if (m_wetDryMixValue == 1)
			{
				// If wet / dry mix is 1, no action is taken
			}

			else
			{
				// If wet/dry mix is between 0 and 1, perform the mixing operation
				// based on dryLevel and wetLevel
				channelData[sample] = channelDataDry[sample] * dryLevel + channelData[sample] * wetLevel;
			}
		}
	}


	//	RMS smoothening
	m_rmsvalueLeft.skip(buffer.getNumSamples());
	m_rmsvalueRight.skip(buffer.getNumSamples());

	{
		const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
		if (value < m_rmsvalueLeft.getCurrentValue())
			m_rmsvalueLeft.setTargetValue(value);
		else
			m_rmsvalueLeft.setCurrentAndTargetValue(value);
	}

	{
		const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
		if (value < m_rmsvalueLeft.getCurrentValue())
			m_rmsvalueRight.setTargetValue(value);
		else
			m_rmsvalueRight.setCurrentAndTargetValue(value);
	}

	//Get RMS value for visualization level meter
	/*m_rmsvalueLeft = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
	m_rmsvalueRight = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));*/
}

//==============================================================================
bool CompressorV4AudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CompressorV4AudioProcessor::createEditor()
{
	return new CompressorV4AudioProcessorEditor(*this);
	//return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void CompressorV4AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	//save parameters
	juce::MemoryOutputStream stream(destData, false);
	m_treeState.state.writeToStream(stream);
}

void CompressorV4AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));

	if (tree.isValid())
	{
		m_treeState.state = tree;
	}
}

float CompressorV4AudioProcessor::getRMSValue(const int channel) const
{
	jassert(channel == 0 || 1);
	if (channel == 0)
		return m_rmsvalueLeft.getCurrentValue();
	if (channel == 1)
		return m_rmsvalueRight.getCurrentValue();
	return 0.f;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new CompressorV4AudioProcessor();
}
