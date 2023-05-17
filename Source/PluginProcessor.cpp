/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ZLInflatorAudioProcessor::ZLInflatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
        : AudioProcessor(
        BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
),
#endif
          parameters(*this, nullptr, juce::Identifier("ZLInflatorParameters"), {}),
          waveShaperAttach(chain.get<waveShaper>(), parameters) {
    parameters.state.addChild(
            {"uiState", {{"width", ZLInterface::WindowWidth}, {"height", ZLInterface::WindowHeight}}, {}}, -1, nullptr);

    parameters.createAndAddParameter(ZLDsp::inputGain::get());
    parameters.createAndAddParameter(ZLDsp::outputGain::get());
    chain.get<gain1>().setGainDecibels(ZLDsp::inputGain::defaultV);
    chain.get<gain2>().setGainDecibels(ZLDsp::outputGain::defaultV);
    parameters.addParameterListener(ZLDsp::inputGain::ID, this);
    parameters.addParameterListener(ZLDsp::outputGain::ID, this);
    waveShaperAttach.addParameters();
}

ZLInflatorAudioProcessor::~ZLInflatorAudioProcessor() = default;

//==============================================================================
const juce::String ZLInflatorAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool ZLInflatorAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ZLInflatorAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ZLInflatorAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ZLInflatorAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int ZLInflatorAudioProcessor::getNumPrograms() {
    return 1; // NB: some hosts don't cope very well if you tell them there are 0
    // programs,
    // so this should be at least 1, even if you're not really implementing
    // programs.
}

int ZLInflatorAudioProcessor::getCurrentProgram() { return 0; }

void ZLInflatorAudioProcessor::setCurrentProgram(int index) {}

const juce::String ZLInflatorAudioProcessor::getProgramName(int index) {
    return {};
}

void ZLInflatorAudioProcessor::changeProgramName(int index,
                                                 const juce::String &newName) {}

//==============================================================================
void ZLInflatorAudioProcessor::prepareToPlay(double sampleRate,
                                             int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need...
    reset();
    auto channels = static_cast<juce::uint32> (juce::jmin(getMainBusNumInputChannels(), getMainBusNumOutputChannels()));
    juce::dsp::ProcessSpec spec{sampleRate, static_cast<juce::uint32> (samplesPerBlock), channels};
    chain.prepare(spec);
    updateParameters();
}

void ZLInflatorAudioProcessor::reset() {
    chain.reset();
}

void ZLInflatorAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations

bool ZLInflatorAudioProcessor::isBusesLayoutSupported(
        const BusesLayout &layouts) const {
    if (layouts.getMainInputChannelSet() != layouts.getMainOutputChannelSet())
        return false;
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

#endif

void ZLInflatorAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                            juce::MidiBuffer &midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    updateParameters();

    juce::dsp::AudioBlock<float> block(buffer);
    chain.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void ZLInflatorAudioProcessor::updateParameters() {
    setLatencySamples(static_cast<int>(chain.get<waveShaper>().getLatencyInSamples()));
}

//==============================================================================
bool ZLInflatorAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *ZLInflatorAudioProcessor::createEditor() {
    return new ZLInflatorAudioProcessorEditor(*this);
//    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void ZLInflatorAudioProcessor::getStateInformation(
        juce::MemoryBlock &destData) {
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ZLInflatorAudioProcessor::setStateInformation(const void *data,
                                                   int sizeInBytes) {
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
    std::unique_ptr<juce::XmlElement> xmlState(
            getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new ZLInflatorAudioProcessor();
}

MeterSource<float> *ZLInflatorAudioProcessor::getInputMeterSource() {
    return &chain.get<meter1>();
}

MeterSource<float> *ZLInflatorAudioProcessor::getOutputMeterSource() {
    return &chain.get<meter2>();
}

shaper::ShaperMixer<float> *ZLInflatorAudioProcessor::getShaperMixer() {
    return chain.get<waveShaper>().getShaper();
}

void ZLInflatorAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue) {
    if (parameterID.equalsIgnoreCase(ZLDsp::inputGain::ID)) {
        chain.get<gain1>().setGainDecibels(newValue);
    } else if (parameterID.equalsIgnoreCase(ZLDsp::outputGain::ID)) {
        chain.get<gain2>().setGainDecibels(newValue);
    }
}