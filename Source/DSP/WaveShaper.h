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

#ifndef ZLWAVESHAPER
#define ZLWAVESHAPER

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"
#include "ShaperFunctions.h"

template<typename FloatType>
class WaveHelper {
public:
    WaveHelper(){
        setWet(static_cast<FloatType>(ZLDsp::wet::formatV(ZLDsp::wet::defaultV)));
    }

    void setWet(FloatType wet) {
        m_wet = wet;
        m_dry = static_cast<FloatType>(1) - wet;
    }

    void setShapes(FloatType curve1, FloatType curve2, FloatType weight) {
        shaperMixer.setShapes(curve1, curve2, weight);
    }

    void setTypes(size_t type1, size_t type2) {
        shaperMixer.setTypes(type1, type2);
    }

    FloatType operator()(FloatType x) const { return shape(x); }

    shaper::ShaperMixer<FloatType>* getShaper() {return &shaperMixer;}

private:
    static constexpr FloatType clip = static_cast<FloatType>(1);
    std::atomic<FloatType> m_wet, m_dry;
    shaper::ShaperMixer<FloatType> shaperMixer;

    static FloatType sgn(FloatType x) {
        if (x > 0) {
            return static_cast<FloatType>(1);
        } else {
            return static_cast<FloatType>(-1);
        }
    }

    FloatType shape(FloatType x) const {
        return shaperMixer(juce::jmin(static_cast<FloatType>(1), std::abs(x))) * m_wet * sgn(x) + x * m_dry;
    }
};

template<typename FloatType>
class LRFilters {
public:
    LRFilters() {
        filters[0].setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
        filters[1].setType(juce::dsp::LinkwitzRileyFilterType::highpass);
        filters[2].setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    }

    void setCutoffFrequency(float freq) {
        if (freq != filters[0].getCutoffFrequency()) {
            for (auto &f: filters) {
                f.setCutoffFrequency(freq);
            }
        }
    }

    void reset() {
        for (auto &f: filters) {
            f.reset();
        }
    }

    void update(const unsigned int factor) {
        if (factor != oldFactor) {
            oldFactor = factor;
            auto rate = (unsigned int) std::pow(2.0, factor);
            juce::dsp::ProcessSpec spec{dupSpec.sampleRate * rate,
                                        dupSpec.numChannels,
                                        dupSpec.maximumBlockSize * rate};
            for (auto &f: filters) {
                f.prepare(spec);
            }
        }
    }

    void prepare(const juce::dsp::ProcessSpec &spec) {
        for (auto &f: filters) {
            f.prepare(spec);
        }
        dupSpec.sampleRate = spec.sampleRate;
        dupSpec.numChannels = spec.numChannels;
        dupSpec.maximumBlockSize = spec.maximumBlockSize;
    }

    template<typename ProcessContext>
    void processLow(ProcessContext &context) {
        filters[0].process(context);
    }

    template<typename ProcessContext>
    void processHigh(ProcessContext &context) {
        filters[1].process(context);
    }

    template<typename ProcessContext>
    void processAll(ProcessContext &context) {
        filters[2].process(context);
    }

private:
    std::atomic<unsigned int> oldFactor = 0;
    std::array<juce::dsp::LinkwitzRileyFilter<FloatType>, 3> filters{};
    juce::dsp::LinkwitzRileyFilter<FloatType> low, high, all;
    juce::dsp::ProcessSpec dupSpec{44100, 0, 2};
};

template<typename FloatType>
class WaveShaper {
public:
    WaveShaper() {
        filters[0].setCutoffFrequency(ZLDsp::lowSplit::defaultV);
        filters[1].setCutoffFrequency(ZLDsp::highSplit::defaultV);
    }

    shaper::ShaperMixer<FloatType>* getShaper() {return helper.getShaper();}

    void setWet(FloatType wet) {
        helper.setWet(wet);
    }

    void setShapes(FloatType curve1, FloatType curve2, FloatType weight) {
        helper.setShapes(curve1, curve2, weight);
    }

    void setCutoffFrequency(FloatType lowFreq, FloatType highFreq) {
        filters[0].setCutoffFrequency(lowFreq);
        filters[1].setCutoffFrequency(highFreq);
    }

    void setEffectFlag(bool effectFlag) {
        effect = effectFlag;
    }

    void setSplitFlag(bool splitFlag) {
        split = splitFlag;
    }

    void setOverSampleFactor(int overSampleFactor) {
        idxSampler = static_cast<size_t>(std::min(overSampleFactor, numSamplers - 1));
    }

    void setTypes(size_t type1, size_t type2) {
        helper.setTypes(type1, type2);
    }

    void setParameters(bool clipFlag, bool effectFlag, float curve,
                       float wetWeight, bool bandSplit, float low_split,
                       float high_split, int overSampleFactor) {
        helper.setParameters(clipFlag, curve, wetWeight);
        effect = effectFlag;
        split = bandSplit;
        idxSampler = (unsigned int) std::min(overSampleFactor, numSamplers - 1);
        filters[0].setCutoffFrequency(low_split);
        filters[1].setCutoffFrequency(high_split);
        for (size_t i = 0; i < numBands - 1; ++i) {
            filters[i].update(idxSampler);
        }
    }

    void reset() noexcept {
        for (size_t i = 0; i < numBands - 1; ++i) {
            filters[i].reset();
        }
        for (size_t i = 0; i < numSamplers; i++) {
            if (overSamplers[i] != nullptr)
                overSamplers[i]->reset();
        }
    }

    float getLatencyInSamples() {
        return overSamplers[idxSampler]->getLatencyInSamples();
    }

    template<typename SampleType>
    SampleType JUCE_VECTOR_CALLTYPE
    processSample(SampleType s) noexcept {
        return helper(s);
    }

    //==============================================================================
    template<typename ProcessContext>
    void process(const ProcessContext &context) noexcept {
        const auto numSamples = context.getInputBlock().getNumSamples();
        const auto numChannels = context.getInputBlock().getNumChannels();
        if (context.isBypassed || !effect) {
            if (context.usesSeparateInputAndOutputBlocks())
                context.getOutputBlock().copyFrom(context.getInputBlock());
        } else {
            auto oversampled_block =
                    overSamplers[idxSampler]->processSamplesUp(context.getInputBlock());
            auto oversampled_context =
                    juce::dsp::ProcessContextReplacing<FloatType>(oversampled_block);
            if (split) {
                auto sepBlock =
                        juce::dsp::AudioBlock<FloatType>(bufferSeparation)
                                .getSubBlock(0, (size_t) ((FloatType) std::pow(2.0, idxSampler) *
                                                          (FloatType) numSamples));
                juce::dsp::AudioBlock<FloatType> blocks[numBands];
                std::vector<juce::dsp::ProcessContextReplacing<FloatType>> contexts;
                for (size_t i = 0; i < numBands; ++i) {
                    blocks[i] = sepBlock.getSubsetChannelBlock(i * numChannels,
                                                               (size_t) numChannels);
                    blocks[i].copyFrom(oversampled_context.getInputBlock());
                    contexts.push_back(
                            juce::dsp::ProcessContextReplacing<FloatType>(blocks[i]));
                }

                filters[0].processLow(contexts[0]);
                filters[1].processAll(contexts[0]);

                filters[0].processHigh(contexts[1]);
                contexts[2].getOutputBlock().copyFrom(contexts[1].getOutputBlock());

                filters[1].processLow(contexts[1]);
                filters[1].processHigh(contexts[2]);

                for (size_t i = 0; i < numBands; ++i) {
                    juce::dsp::AudioBlock<FloatType>::process(
                            contexts[i].getInputBlock(), contexts[i].getOutputBlock(),
                            helper);
                }

                for (size_t i = 1; i < numBands; ++i) {
                    blocks[0].add(blocks[i]);
                }

                oversampled_context.getOutputBlock().copyFrom(blocks[0]);
            } else {
                juce::dsp::AudioBlock<FloatType>::process(
                        oversampled_context.getInputBlock(),
                        oversampled_context.getOutputBlock(), helper);
            }
            overSamplers[idxSampler]->processSamplesDown(context.getOutputBlock());
        }
    }

    void prepare(const juce::dsp::ProcessSpec &spec) {
        reset();
        sampleRate = spec.sampleRate;
        for (size_t i = 0; i < numBands - 1; ++i) {
            filters[i].prepare(spec);
        }
        bufferSeparation.setSize((int) spec.numChannels * numBands,
                                 int(16 * spec.maximumBlockSize), false, false,
                                 true);
        for (size_t i = 0; i < numSamplers; ++i) {
            overSamplers[i] = std::make_unique<juce::dsp::Oversampling<FloatType >>(
                    spec.numChannels, i,
                    juce::dsp::Oversampling<FloatType>::filterHalfBandFIREquiripple, true,
                    true);
            overSamplers[i]->initProcessing(spec.maximumBlockSize);
        }
    }

private:
    constexpr static const int numSamplers = 5, numBands = 3;
    std::atomic<double> sampleRate;
    WaveHelper<FloatType> helper;
    std::array<std::unique_ptr<juce::dsp::Oversampling<FloatType>>, numSamplers>
            overSamplers{};
    std::atomic<size_t> idxSampler = ZLDsp::overSample::defaultI;
    std::atomic<bool> split = ZLDsp::bandSplit::defaultV, effect = ZLDsp::effectIn::defaultV;
    std::array<LRFilters<FloatType>, numBands - 1> filters{};
    juce::AudioBuffer<FloatType> bufferSeparation;
};

template<typename FloatType>
class WaveShaperAttach : public juce::AudioProcessorValueTreeState::Listener {
public:
    explicit WaveShaperAttach(WaveShaper<FloatType> &shaper, juce::AudioProcessorValueTreeState &parameters) {
        waveShaper = &shaper;
        apvts = &parameters;
    }

    void addParameters() {
        std::array IDs{ZLDsp::effectIn::ID, ZLDsp::style1::ID, ZLDsp::style2::ID,
                       ZLDsp::wet::ID, ZLDsp::curve1::ID, ZLDsp::curve2::ID, ZLDsp::weight::ID,
                       ZLDsp::bandSplit::ID, ZLDsp::lowSplit::ID, ZLDsp::highSplit::ID,
                       ZLDsp::overSample::ID};
        apvts->createAndAddParameter(ZLDsp::effectIn::get());
        apvts->createAndAddParameter(ZLDsp::style1::get());
        apvts->createAndAddParameter(ZLDsp::style2::get());

        apvts->createAndAddParameter(ZLDsp::wet::get());

        apvts->createAndAddParameter(ZLDsp::curve1::get());
        apvts->createAndAddParameter(ZLDsp::curve2::get());
        apvts->createAndAddParameter(ZLDsp::weight::get());

        apvts->createAndAddParameter(ZLDsp::bandSplit::get());
        apvts->createAndAddParameter(ZLDsp::lowSplit::get());
        apvts->createAndAddParameter(ZLDsp::highSplit::get());

        apvts->createAndAddParameter(ZLDsp::overSample::get());

        for (auto &ID: IDs) {
            apvts->addParameterListener(ID, this);
        }
    }

    void parameterChanged(const juce::String &parameterID, float newValue) override {
        if (parameterID == ZLDsp::wet::ID) {
            waveShaper->setWet(static_cast<FloatType>(newValue) / static_cast<FloatType>(100));
        } else if (parameterID == ZLDsp::curve1::ID || parameterID == ZLDsp::curve2::ID ||
                   parameterID == ZLDsp::weight::ID) {
            auto curve1 = apvts->getRawParameterValue(ZLDsp::curve1::ID)->load();
            auto curve2 = apvts->getRawParameterValue(ZLDsp::curve2::ID)->load();
            auto weight = apvts->getRawParameterValue(ZLDsp::weight::ID)->load();
            if (parameterID == ZLDsp::curve1::ID) {
                curve1 = newValue;
            } else if (parameterID == ZLDsp::curve2::ID) {
                curve2 = newValue;
            } else {
                weight = newValue;
            }
            waveShaper->setShapes(static_cast<FloatType>(ZLDsp::curve1::formatV(curve1)),
                                  static_cast<FloatType>(ZLDsp::curve2::formatV(curve2)),
                                  static_cast<FloatType>(ZLDsp::weight::formatV(weight)));
        } else if (parameterID == ZLDsp::lowSplit::ID || parameterID == ZLDsp::highSplit::ID) {
            auto lowSplit = apvts->getRawParameterValue(ZLDsp::lowSplit::ID)->load();
            auto highSplit = apvts->getRawParameterValue(ZLDsp::highSplit::ID)->load();
            if (parameterID == ZLDsp::lowSplit::ID) {
                lowSplit = newValue;
            } else {
                highSplit = newValue;
            }
            waveShaper->setCutoffFrequency(lowSplit, highSplit);
        } else if (parameterID == ZLDsp::bandSplit::ID) {
            waveShaper->setSplitFlag(static_cast<bool>(newValue));
        } else if (parameterID == ZLDsp::effectIn::ID) {
            waveShaper->setEffectFlag(static_cast<bool>(newValue));
        } else if (parameterID == ZLDsp::style1::ID || parameterID == ZLDsp::style2::ID) {
            auto type1 = apvts->getRawParameterValue(ZLDsp::style1::ID)->load();
            auto type2 = apvts->getRawParameterValue(ZLDsp::style2::ID)->load();
            if (parameterID == ZLDsp::style1::ID) {
                type1 = newValue;
            } else {
                type2 = newValue;
            }
            waveShaper->setTypes(static_cast<size_t>(type1), static_cast<size_t>(type2));
        } else if (parameterID == ZLDsp::overSample::ID) {
            waveShaper->setOverSampleFactor(static_cast<int>(newValue));
        }
    }

private:
    WaveShaper<FloatType> *waveShaper;
    juce::AudioProcessorValueTreeState *apvts;
};

#endif