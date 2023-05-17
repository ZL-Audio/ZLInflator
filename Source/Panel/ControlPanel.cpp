/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "ControlPanel.h"

ControlPanel::ControlPanel(juce::AudioProcessorValueTreeState &apvts) {
    // init sliders
    std::array sliderList{&inputGainSlider, &outputGainSlider, &lowSplitSlider, &highSplitSlider, &wetSlider,
                          &curve1Slider, &weightSlider, &curve2Slider};
    std::array sliderID{ZLDsp::inputGain::ID, ZLDsp::outputGain::ID, ZLDsp::lowSplit::ID, ZLDsp::highSplit::ID,
                        ZLDsp::wet::ID, ZLDsp::curve1::ID, ZLDsp::weight::ID, ZLDsp::curve2::ID};
    for (size_t i = 0; i < sliderList.size(); ++i) {
        *sliderList[i] = std::make_unique<RotarySliderComponent>(apvts.getParameter(sliderID[i])->name);
        addAndMakeVisible(*(*sliderList[i]));
        sliderAttachments.add(
                std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, sliderID[i],
                                                                                       (*sliderList[i])->getSlider()));
    }
    // init buttons
    std::array buttonList{&effectButton, &splitButton};
    std::array buttonID{ZLDsp::effectIn::ID, ZLDsp::bandSplit::ID};
    for (size_t i = 0; i < buttonList.size(); ++i) {
        *buttonList[i] = std::make_unique<ButtonComponent>(apvts.getParameter(buttonID[i])->name);
        addAndMakeVisible(*(*buttonList[i]));
        buttonAttachments.add(
                std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, buttonID[i],
                                                                                       (*buttonList[i])->getButton()));
    }

    // init combobox
    std::array comboboxList{&style1Box, &style2Box};
    std::array comboboxID{ZLDsp::style1::ID, ZLDsp::style2::ID};
    for (size_t i = 0; i < comboboxList.size(); ++i) {
        *comboboxList[i] = std::make_unique<ComboboxComponent>(apvts.getParameter(comboboxID[i])->name,
                                                               apvts.getParameter(comboboxID[i])->getAllValueStrings());
        addAndMakeVisible(*(*comboboxList[i]));
        comboboxAttachments.add(
                std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, comboboxID[i],
                                                                                         (*comboboxList[i])->getComboBox()));
    }
}

ControlPanel::~ControlPanel() = default;

//==============================================================================
void ControlPanel::paint(juce::Graphics &g) {}

void ControlPanel::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(2)), Track(Fr(3)), Track(Fr(3))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    juce::Array<juce::GridItem> items;

    items.add(*effectButton);
    items.add(*splitButton);
    items.add(*style1Box);
    items.add(*style2Box);
    items.add(*inputGainSlider);
    items.add(*lowSplitSlider);
    items.add(*wetSlider);
    items.add(*weightSlider);
    items.add(*outputGainSlider);
    items.add(*highSplitSlider);
    items.add(*curve1Slider);
    items.add(*curve2Slider);

    grid.items = items;

    grid.performLayout(getLocalBounds());
}

void ControlPanel::setFontSize(float size) {
    std::array sliderList{&inputGainSlider, &outputGainSlider, &lowSplitSlider, &highSplitSlider, &wetSlider,
                          &curve1Slider, &weightSlider, &curve2Slider};
    for (auto const &s: sliderList) {
        (*s)->setFontSize(size);
    }
    std::array buttonList{&effectButton, &splitButton};
    for (auto const &b: buttonList) {
        (*b)->setFontSize(size);
    }
    std::array comboboxList{&style1Box, &style2Box};
    for (auto const &c: comboboxList) {
        (*c)->setFontSize(size);
    }
}