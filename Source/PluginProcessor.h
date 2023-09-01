/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#pragma once

#include "DSP/dsp_defines.h"
#include "DSP/MeterSource.h"
#include "DSP/WaveShaper.h"
#include "GUI/interface_definitions.h"
#include "State/dummy_processor.h"
#include "State/state_definitions.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================
/**
 */
class ZLInflatorAudioProcessor : public juce::AudioProcessor,
                                 public juce::AudioProcessorValueTreeState::Listener
#if JucePlugin_Enable_ARA
    ,
                                 public juce::AudioProcessorARAExtension
#endif
{
public:
    DummyProcessor dummyProcessor;
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioProcessorValueTreeState states;

    //==============================================================================
    ZLInflatorAudioProcessor();

    ~ZLInflatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

#endif

    void process(juce::dsp::ProcessContextReplacing<float> context);

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    void reset() override;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;

    bool producesMidi() const override;

    bool isMidiEffect() const override;

    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;

    int getCurrentProgram() override;

    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override;

    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;

    void setStateInformation(const void *data, int sizeInBytes) override;

    MeterSource<float> *getInputMeterSource();

    MeterSource<float> *getOutputMeterSource();

    shaper::ShaperMixer<float> *getShaperMixer();

    void parameterChanged(const juce::String &parameterID, float newValue) override;


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZLInflatorAudioProcessor)

    juce::dsp::Gain<float> inGain, outGain;
    MeterSource<float> meterIn, meterOut;
    WaveShaper<float> waveShaper;
    WaveShaperAttach<float> waveShaperAttach;
};
