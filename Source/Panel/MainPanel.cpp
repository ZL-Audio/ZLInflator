/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "MainPanel.h"

MainPanel::MainPanel(juce::AudioProcessorValueTreeState &apvts,
                     MeterSource<float> *input,
                     MeterSource<float> *output,
                     shaper::ShaperMixer<float> *shaperMixer) :
        controlPanel(apvts),
        topPanel(apvts),
        meterPanel(input, output),
        plotPanel(shaperMixer) {
    addAndMakeVisible(controlPanel);
    addAndMakeVisible(topPanel);
    addAndMakeVisible(meterPanel);
    addAndMakeVisible(plotPanel);
}

MainPanel::~MainPanel() = default;

void MainPanel::paint(juce::Graphics &g) {
    g.fillAll(ZLInterface::BackgroundColor);
    auto bound = getLocalBounds().toFloat();
    auto padding = bound.getHeight() * 0.05f;
    bound = bound.withSizeKeepingCentre(bound.getWidth() - padding, bound.getHeight() - padding);
    float cornerSize = bound.getHeight() * 0.03f;
    ZLInterface::fillRoundedRectangle(g, bound, cornerSize);
}

void MainPanel::resized() {
    topPanel.setFontSize(static_cast<float>(getLocalBounds().getHeight()) * 0.045f);
    controlPanel.setFontSize(static_cast<float>(getLocalBounds().getHeight()) * 0.045f);
    meterPanel.setFontSize(static_cast<float>(getLocalBounds().getHeight()) * 0.045f);
    plotPanel.setFontSize(static_cast<float>(getLocalBounds().getHeight()) * 0.045f);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1)), Track(Fr(3))};
    grid.templateColumns = {Track(Fr(6)), Track(Fr(10)), Track(Fr(3))};

    grid.items = {
            juce::GridItem(topPanel).withArea(1, 1, 2, 2),
            juce::GridItem(plotPanel).withArea(2, 1, 3, 2),
            juce::GridItem(controlPanel).withArea(1, 2, 3, 3),
            juce::GridItem(meterPanel).withArea(1, 3, 3, 4),
    };

    auto bound = getLocalBounds().toFloat();
    auto padding = bound.getHeight() * 0.08f;
    bound = bound.withSizeKeepingCentre(bound.getWidth() - padding, bound.getHeight() - padding);
    grid.performLayout(bound.toNearestInt());


}

void MainPanel::setFontSize(float size) {
    topPanel.setFontSize(size);
    controlPanel.setFontSize(size);
    meterPanel.setFontSize(size);
}