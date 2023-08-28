// ==============================================================================
// Copyright (C) 2023 - zsliu98
// This file is part of ZLEComp
//
// ZLEComp is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// ZLEComp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with ZLEComp. If not, see <https://www.gnu.org/licenses/>.
// ==============================================================================

#ifndef ZL_PANEL_DEFINITIONS_H
#define ZL_PANEL_DEFINITIONS_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "../GUI/interface_definitions.h"

namespace zlpanel {
    template<class T1, size_t N>
    inline void attachSliders(juce::Component &mainComponent,
                              std::array<std::unique_ptr<T1> *, N> components,
                              juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> &attachments,
                              std::array<std::string, N> &ids,
                              juce::AudioProcessorValueTreeState &parameters,
                              zlinterface::UIBase &base) {
        for (size_t i = 0; i < N; ++i) {
            *components[i] = std::make_unique<T1>(
                    parameters.getParameter(ids[i])->label, base);
            mainComponent.addAndMakeVisible(*(*components[i]));
            attachments.add(
                    std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                            parameters, ids[i], (*components[i])->getSlider()));
        }
    }

    template<class T1, size_t N>
    inline void attachBoxes(juce::Component &mainComponent,
                            std::array<std::unique_ptr<T1> *, N> components,
                            juce::OwnedArray<juce::AudioProcessorValueTreeState::ComboBoxAttachment> &attachments,
                            std::array<std::string, N> &ids,
                            juce::AudioProcessorValueTreeState &parameters,
                            zlinterface::UIBase &base) {
        for (size_t i = 0; i < N; ++i) {
            *components[i] = std::make_unique<T1>(
                    parameters.getParameter(ids[i])->label,
                    parameters.getParameter(ids[i])->getAllValueStrings(),
                    base);
            mainComponent.addAndMakeVisible(*(*components[i]));
            attachments.add(
                    std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
                            parameters, ids[i], (*components[i])->getComboBox()));
        }
    }

    template<class T1, size_t N>
    inline void attachButtons(juce::Component &mainComponent,
                              std::array<std::unique_ptr<T1> *, N> components,
                              juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> &attachments,
                              std::array<std::string, N> &ids,
                              juce::AudioProcessorValueTreeState &parameters,
                              zlinterface::UIBase &base) {
        for (size_t i = 0; i < N; ++i) {
            *components[i] = std::make_unique<T1>(parameters.getParameter(ids[i])->label, base);
            mainComponent.addAndMakeVisible(*(*components[i]));
            attachments.add(
                    std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
                            parameters, ids[i], (*components[i])->getButton()));
        }
    }
}

#endif //ZL_PANEL_DEFINITIONS_H
