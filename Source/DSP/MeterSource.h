/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_METERSOURCE_H
#define ZLINFLATOR_METERSOURCE_H

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

template<typename FloatType>
class MeterSource
{
public:
    void reset() noexcept {}

    template<typename SampleType>
    SampleType JUCE_VECTOR_CALLTYPE processSample(SampleType s) noexcept {
        return s;
    }

    template<typename ProcessContext>
    void process(const ProcessContext &context) noexcept {
        if (context.usesSeparateInputAndOutputBlocks())
            context.getOutputBlock().copyFrom(context.getInputBlock());
        if (lock) {
            return;
        }
        const auto numSamples = context.getInputBlock().getNumSamples();
        const auto numChannels = context.getInputBlock().getNumChannels();
        auto block = context.getInputBlock();
        for (size_t i = 0; i < numChannels; ++i) {
            auto currentRMS = juce::Decibels::gainToDecibels(getRMSLevel(block, i, 0, numSamples));
            auto currentPeak = juce::Decibels::gainToDecibels(getPeakLevel(block, i, 0, numSamples));
            bufferRMS[i] = juce::jmax(bufferRMS[i], currentRMS);
            bufferPeak[i] = juce::jmax(bufferPeak[i], currentPeak);
            peakMax[i] = juce::jmax(currentPeak, peakMax[i]);
        }
    }

    void prepare(const juce::dsp::ProcessSpec &spec) {

        for (auto f: {&peakMax, &bufferRMS, &bufferPeak, &displayRMS, &displayPeak}) {
            (*f).resize(spec.numChannels);
        }
        resetPeakMax();
        resetBuffer();
        for (size_t i = 0; i < displayRMS.size(); ++i) {
            displayRMS[i] = static_cast<FloatType>(-100);
            displayPeak[i] = static_cast<FloatType>(-100);
        }
    }

    std::vector<FloatType> getDisplayRMS() {
        for (size_t i = 0; i < displayRMS.size(); ++i) {
            displayRMS[i] = juce::jmax(displayRMS[i] - static_cast<FloatType>(decayRate), bufferRMS[i]);
        }
        return displayRMS;
    }

    std::vector<FloatType> getDisplayPeak() {
        for (size_t i = 0; i < displayPeak.size(); ++i) {
            displayPeak[i] = juce::jmax(displayPeak[i] - static_cast<FloatType>(decayRate), bufferPeak[i]);
        }
        return displayPeak;
    }

    std::vector<FloatType> getPeakMax() {
        return peakMax;
    }

    void resetBuffer() {
        lock = true;
        for (size_t i = 0; i < bufferRMS.size(); ++i) {
            bufferRMS[i] = static_cast<FloatType>(-100);
            bufferPeak[i] = static_cast<FloatType>(-100);
        }
        lock = false;
    }

    void resetPeakMax() {
        lock = true;
        for (size_t i = 0; i < peakMax.size(); ++i) {
            peakMax[i] = static_cast<FloatType>(-100);
        }
        lock = false;
    }

    void setDecayRate(float x) {
        decayRate = x;
    }

    bool getDataFlag() {
        return dataFlag;
    }

    void resetDataFlag() {
        dataFlag = false;
    }

private:
    std::vector<FloatType> peakMax;
    std::vector<FloatType> bufferRMS, bufferPeak;
    std::vector<FloatType> displayRMS, displayPeak;
    std::atomic<bool> lock = false;
    float decayRate = 0.12f;
    bool dataFlag = false;

    template<typename T>
    T getRMSLevel(juce::dsp::AudioBlock<T> block, unsigned long channel, unsigned long startSample,
                  unsigned long numSamples) const noexcept {
        if (numSamples <= 0 || channel < 0 || channel >= block.getNumChannels() || block.getNumSamples() == 0)
            return FloatType(0);

        auto *data = block.getChannelPointer(channel) + startSample;
        double sum = 0.0;

        for (size_t i = 0; i < numSamples; ++i) {
            auto sample = data[i];
            sum += sample * sample;
        }
        return static_cast<FloatType> (std::sqrt(sum / static_cast<double>(numSamples)));
    }

    template<typename T>
    T getPeakLevel(juce::dsp::AudioBlock<T> block, unsigned long channel, unsigned long startSample,
                   unsigned long numSamples) const noexcept {
        if (numSamples <= 0 || channel < 0 || channel >= block.getNumChannels() || block.getNumSamples() == 0)
            return FloatType(0);

        auto *data = block.getChannelPointer(channel) + startSample;
        FloatType localPeak = -1000.0;

        for (size_t i = 0; i < numSamples; ++i) {
            if (data[i] > localPeak) {
                localPeak = static_cast<FloatType> (data[i]);
            }
        }
        return localPeak;
    }
};

#endif
