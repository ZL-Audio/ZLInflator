/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_DSP_DEFINES_H
#define ZLINFLATOR_DSP_DEFINES_H

#include <juce_audio_processors/juce_audio_processors.h>

namespace ZLDsp {
    // floats
    template<class T>
    class FloatParameters {
    public:
        static std::unique_ptr<juce::AudioParameterFloat> get() {
            return std::make_unique<juce::AudioParameterFloat>(T::ID, T::name, T::range, T::defaultV);
        }
    };

    class inputGain : public FloatParameters<inputGain> {
    public:
        auto static constexpr ID = "input_gain";
        auto static constexpr name = "IN Gain (dB)";
        inline auto static const range = juce::NormalisableRange<float>(-6.0f, 6.0f, 0.1f);
        auto static constexpr defaultV = 0.0f;
    };

    class outputGain : public FloatParameters<outputGain> {
    public:
        auto static constexpr ID = "output_gain";
        auto static constexpr name = "OUT Gain (dB)";
        inline auto static const range = juce::NormalisableRange<float>(-10.0f, 2.0f, 0.1f);
        auto static constexpr defaultV = 0.0f;
    };

    class wet : public FloatParameters<wet> {
    public:
        auto static constexpr ID = "wet";
        auto static constexpr name = "Wet (%)";
        inline auto static const range = juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f);
        auto static constexpr defaultV = 100.0f;
        static float formatV(float v) {
            return v / 100.f;
        }
    };

    class curve1 : public FloatParameters<curve1> {
    public:
        auto static constexpr ID = "curve1";
        auto static constexpr name = "Curve (%)";
        inline auto static const range = juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f);
        auto static constexpr defaultV = 50.0f;
        static float formatV(float v) {
            return v / 100.f;
        }
    };

    class curve2 : public FloatParameters<curve2> {
    public:
        auto static constexpr ID = "curve2";
        auto static constexpr name = "Curve (%)";
        inline auto static const range = juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f);
        auto static constexpr defaultV = 50.0f;
        static float formatV(float v) {
            return v / 100.f;
        }
    };

    class weight : public FloatParameters<weight> {
    public:
        auto static constexpr ID = "weight";
        auto static constexpr name = "Weight (%)";
        inline auto static const range = juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f);
        auto static constexpr defaultV = 50.0f;
        static float formatV(float v) {
            return v / 100.f;
        }
    };

    class lowSplit : public FloatParameters<lowSplit> {
    public:
        auto static constexpr ID = "low_split";
        auto static constexpr name = "Low Split (Hz)";
        inline auto static const range = juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.2f);
        auto static constexpr defaultV = 240.0f;
    };

    class highSplit : public FloatParameters<highSplit> {
    public:
        auto static constexpr ID = "high_split";
        auto static constexpr name = "High Split (Hz)";
        inline auto static const range = juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.2f);
        auto static constexpr defaultV = 2400.0f;
    };

    // bools
    template<class T>
    class BoolParameters {
    public:
        static std::unique_ptr<juce::AudioParameterBool> get() {
            return std::make_unique<juce::AudioParameterBool>(T::ID, T::name, T::defaultV);
        }
    };

    class effectIn : public BoolParameters<effectIn> {
    public:
        auto static constexpr ID = "effect_in";
        auto static constexpr name = "Effect In";
        auto static constexpr defaultV = true;
    };

    class bandSplit : public BoolParameters<bandSplit> {
    public:
        auto static constexpr ID = "band_split";
        auto static constexpr name = "Band Split";
        auto static constexpr defaultV = false;
    };

    // choices
    template<class T>
    class ChoiceParameters {
    public:
        static std::unique_ptr<juce::AudioParameterChoice> get() {
            return std::make_unique<juce::AudioParameterChoice>(T::ID, T::name, T::choices, T::defaultI);
        }
    };

    class overSample : public ChoiceParameters<overSample> {
    public:
        auto static constexpr ID = "over_sample";
        auto static constexpr name = "Over Sampling";
        inline auto static const choices = juce::StringArray{"OFF", "2x", "4x", "8x", "16x"};
        int static constexpr defaultI = 0;
    };

    class style1 : public ChoiceParameters<style1> {
    public:
        auto static constexpr ID = "style1";
        auto static constexpr name = "1st Style";
        inline auto static const choices = juce::StringArray{"Identity", "Quadratic", "Cubic", "Quartic", "Sigmod", "Sin"};
        int static constexpr defaultI = 1;
    };

    class style2 : public ChoiceParameters<style2> {
    public:
        auto static constexpr ID = "style2";
        auto static constexpr name = "2nd Style";
        inline auto static const choices = juce::StringArray{"Identity", "Quadratic", "Cubic", "Quartic", "Sigmod", "Sin"};
        int static constexpr defaultI = 2;
    };
}

#endif //ZLINFLATOR_DSP_DEFINES_H
