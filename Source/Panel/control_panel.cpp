/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "control_panel.h"

ControlPanel::ControlPanel(juce::AudioProcessorValueTreeState &apvts, zlinterface::UIBase &base) {
    // init sliders
    std::array<std::string, 8> rotarySliderID{zldsp::inputGain::ID, zldsp::outputGain::ID, zldsp::lowSplit::ID, zldsp::highSplit::ID,
                        zldsp::wet::ID, zldsp::curve1::ID, zldsp::weight::ID, zldsp::curve2::ID};
    zlpanel::attachSliders<zlinterface::RotarySliderComponent, 8>(*this, rotarySliderList, sliderAttachments, rotarySliderID,
                                                         apvts, base);
    // init buttons
    std::array<std::string, 2> buttonID{zldsp::effectIn::ID, zldsp::bandSplit::ID};
    zlpanel::attachButtons(*this, buttonList, buttonAttachments, buttonID, apvts, base);

    // init combobox
    std::array<std::string, 2> comboboxID{zldsp::style1::ID, zldsp::style2::ID};
    zlpanel::attachBoxes(*this, comboBoxList, comboboxAttachments, comboboxID, apvts, base);
}

ControlPanel::~ControlPanel() = default;

//==============================================================================
void ControlPanel::paint(juce::Graphics &g) {}

void ControlPanel::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1)), Track(Fr(2)), Track(Fr(2))};
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