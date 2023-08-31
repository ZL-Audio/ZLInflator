// ==============================================================================
// Copyright (C) 2023 - zsliu98
// This file is part of ZLEComp
//
// ZLEComp is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// ZLEComp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with ZLEComp. If not, see <https://www.gnu.org/licenses/>.
// ==============================================================================

#ifndef ZLECOMP_STATE_DEFINITIONS_H
#define ZLECOMP_STATE_DEFINITIONS_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <BinaryData.h>

namespace zlstate {
    // int
    inline auto static const versionHint = 1;

    template<class T>
    class IntParameters {
    public:
        static std::unique_ptr<juce::AudioParameterInt> get(bool automate = true) {
            auto attributes = juce::AudioParameterIntAttributes().withAutomatable(automate).withLabel(T::name);
            return std::make_unique<juce::AudioParameterInt>(juce::ParameterID(T::ID, versionHint), T::name,
                                                             T::minV, T::maxV, T::defaultV, attributes);
        }

        inline static float convertTo01(int x) {
            return static_cast<float>(x - T::minV) / static_cast<float>(T::maxV - T::minV);
        }
    };

    template<class T>
    class FloatParameters {
    public:
        static std::unique_ptr<juce::AudioParameterFloat> get(bool automate = true) {
            auto attributes = juce::AudioParameterFloatAttributes().withAutomatable(automate).withLabel(T::name);
            return std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(T::ID, versionHint), T::name,
                                                             T::range, T::defaultV, attributes);
        }

        inline static float convertTo01(float x) {
            return T::range.convertTo0to1(x);
        }
    };

    class uiStyle : public FloatParameters<uiStyle> {public:
        auto static constexpr ID = "ui_style";
        auto static constexpr name = "NA";
        inline static const int minV = 0;
        inline static const int maxV = 1;
        inline static const int defaultV = 1;
        inline auto static const range =
                juce::NormalisableRange<float>(minV, maxV, 1.f);
    };

    class windowW : public FloatParameters<windowW> {
    public:
        auto static constexpr ID = "window_w";
        auto static constexpr name = "NA";
        inline static const int minV = 360;
        inline static const int maxV = 3600;
        inline static const int defaultV = 511;
        inline auto static const range =
                juce::NormalisableRange<float>(minV, maxV, 1.f);
    };

    class windowH : public FloatParameters<windowH> {
    public:
        auto static constexpr ID = "window_h";
        auto static constexpr name = "NA";
        inline static const int minV = 150;
        inline static const int maxV = 1500;
        inline static const int defaultV = 213;
        inline auto static const range =
                juce::NormalisableRange<float>(minV, maxV, 1.f);
    };

    // bool
    template<class T>
    class BoolParameters {
    public:
        static std::unique_ptr<juce::AudioParameterBool> get(bool automate = true) {
            auto attributes = juce::AudioParameterBoolAttributes().withAutomatable(automate).withLabel(T::name);
            return std::make_unique<juce::AudioParameterBool>(juce::ParameterID(T::ID, versionHint), T::name,
                                                              T::defaultV, attributes);
        }

        static std::unique_ptr<juce::AudioParameterBool> get(juce::String label, bool automate = true) {
            auto attributes = juce::AudioParameterBoolAttributes().withAutomatable(automate).withLabel(label);
            return std::make_unique<juce::AudioParameterBool>(juce::ParameterID(T::ID, versionHint), T::name,
                                                              T::defaultV, attributes);
        }
    };

    // choice
    template<class T>
    class ChoiceParameters {
    public:
        static std::unique_ptr<juce::AudioParameterChoice> get(bool automate = true) {
            auto attributes = juce::AudioParameterChoiceAttributes().withAutomatable(automate).withLabel(T::name);
            return std::make_unique<juce::AudioParameterChoice>(
                    juce::ParameterID(T::ID, versionHint), T::name, T::choices, T::defaultI, attributes);
        }

        static std::unique_ptr<juce::AudioParameterChoice> get(juce::String label, bool automate = true) {
            auto attributes = juce::AudioParameterChoiceAttributes().withAutomatable(automate).withLabel(label);
            return std::make_unique<juce::AudioParameterChoice>(
                    juce::ParameterID(T::ID, versionHint), T::name, T::choices, T::defaultI, attributes);
        }
    };

    inline juce::AudioProcessorValueTreeState::ParameterLayout getParameterLayout() {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;
        layout.add(uiStyle::get(false),
                   windowW::get(false), windowH::get(false));
        return layout;
    }
}

#endif //ZLECOMP_STATE_DEFINITIONS_H
