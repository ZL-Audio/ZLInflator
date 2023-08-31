/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "main_panel.h"

MainPanel::MainPanel(ZLInflatorAudioProcessor &p,
                     MeterSource<float> *input,
                     MeterSource<float> *output,
                     shaper::ShaperMixer<float> *shaperMixer) :
        uiBase(),
        controlPanel(p.parameters, uiBase),
        topPanel(p, uiBase),
        meterPanel(input, output, uiBase),
        plotPanel(p, shaperMixer, uiBase) {
    addAndMakeVisible(controlPanel);
    addAndMakeVisible(topPanel);
    addAndMakeVisible(meterPanel);
    addAndMakeVisible(plotPanel);
}

MainPanel::~MainPanel() = default;

void MainPanel::paint(juce::Graphics &g) {
    g.fillAll(uiBase.getBackgroundColor());
    auto bound = getLocalBounds().toFloat();
    float fontSize = bound.getHeight() * 0.048f;
    bound = uiBase.fillRoundedShadowRectangle(g, bound, fontSize * 0.5f, {});
    uiBase.fillRoundedInnerShadowRectangle(g, bound, fontSize * 0.5f, {.blurRadius=0.45f, .flip=true});
}

void MainPanel::resized() {
    auto bound = getLocalBounds().toFloat();
    auto fontSize = bound.getHeight() * 0.048f;
    bound = zlinterface::getRoundedShadowRectangleArea(bound, fontSize * 0.5f, {});
    bound = zlinterface::getRoundedShadowRectangleArea(bound, fontSize * 0.5f, {});

    uiBase.setFontSize(fontSize);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(3)), Track(Fr(12))};
    grid.templateColumns = {Track(Fr(6)), Track(Fr(10)), Track(Fr(3))};

    grid.items = {
            juce::GridItem(topPanel).withArea(1, 1, 2, 2),
            juce::GridItem(plotPanel).withArea(2, 1, 3, 2),
            juce::GridItem(controlPanel).withArea(1, 2, 3, 3),
            juce::GridItem(meterPanel).withArea(1, 3, 3, 4),
    };

    grid.performLayout(bound.toNearestInt());
}