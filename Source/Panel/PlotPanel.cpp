/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "PlotPanel.h"

PlotPanel::PlotPanel(shaper::ShaperMixer<float> *shaperMixer) :
        shaperPlotComponent((shaperMixer)){
    addAndMakeVisible(shaperPlotComponent);
}

PlotPanel::~PlotPanel() = default;

void PlotPanel::paint(juce::Graphics &) {}

void PlotPanel::resized() {
    auto bound = getLocalBounds().toFloat();
    auto minWH = juce::jmin(bound.getWidth(), bound.getHeight()) * 0.95f;
    bound = bound.withSizeKeepingCentre(minWH, minWH);
    shaperPlotComponent.setBounds(bound.toNearestInt());
}

void PlotPanel::setFontSize(float size) {
    shaperPlotComponent.setFontSize(size);
}
