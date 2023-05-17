/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "MeterPanel.h"

MeterPanel::MeterPanel(MeterSource<float> *input, MeterSource<float> *output) :
inputMeter("IN", input, -40.0f, 0.0f),
outputMeter("OUT", output, -40.0f, 0.0f){
    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);
}

MeterPanel::~MeterPanel() = default;

void MeterPanel::paint(juce::Graphics &) {}

void MeterPanel::resized() {
    auto bound = getLocalBounds().toFloat();
    auto inputBound = bound.removeFromLeft(bound.getWidth() * 0.5f);
    inputMeter.setBounds(inputBound.toNearestInt());
    outputMeter.setBounds(bound.toNearestInt());
}

void MeterPanel::setFontSize(float size) {
    inputMeter.setFontSize(size);
    outputMeter.setFontSize(size);
}
