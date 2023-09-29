/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "meter_panel.h"

MeterPanel::MeterPanel(MeterSource<float> *input, MeterSource<float> *output, zlinterface::UIBase &base) :
        inputBackground("IN", base),
        outputBackground("OUT", base),
        inputMeter(input, -40.0f, 0.0f, base),
        outputMeter(output, -40.0f, 0.0f, base) {
    addAndMakeVisible(inputBackground);
    addAndMakeVisible(outputBackground);
    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);
}

MeterPanel::~MeterPanel() = default;

void MeterPanel::paint(juce::Graphics &) {}

void MeterPanel::resized() {
    auto bound = getLocalBounds().toFloat();
    auto inputBound = bound.removeFromLeft(bound.getWidth() * 0.5f);
    inputBackground.setBounds(inputBound.toNearestInt());
    outputBackground.setBounds(bound.toNearestInt());
    inputMeter.setBounds(inputBound.toNearestInt());
    outputMeter.setBounds(bound.toNearestInt());
}