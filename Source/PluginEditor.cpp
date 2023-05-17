/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "PluginEditor.h"
#include <BinaryData.h>

//==============================================================================
ZLInflatorAudioProcessorEditor::ZLInflatorAudioProcessorEditor(ZLInflatorAudioProcessor &p) :
        AudioProcessorEditor(&p), audioProcessor(p),
        mainPanel(p.parameters, p.getInputMeterSource(), p.getOutputMeterSource(), p.getShaperMixer()) {
    // set font
    auto sourceCodePro = juce::Typeface::createSystemTypefaceFor(BinaryData::OpenSansSemiBold_ttf,
                                                                 BinaryData::OpenSansSemiBold_ttfSize);
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(sourceCodePro);

    // add main panel
    addAndMakeVisible(mainPanel);

    // set size & size listener
    setResizeLimits(ZLInterface::WindowMinWidth, ZLInterface::WindowMinHeight,
                    ZLInterface::WindowMaxWidth, ZLInterface::WindowMaxHeight);
    getConstrainer()->setFixedAspectRatio(ZLInterface::WindowFixedAspectRatio);
    setResizable(true, p.wrapperType != ZLInflatorAudioProcessor::wrapperType_AudioUnitv3);
    lastUIWidth.referTo(p.parameters.state.getChildWithName("uiState").getPropertyAsValue("width", nullptr));
    lastUIHeight.referTo(p.parameters.state.getChildWithName("uiState").getPropertyAsValue("height", nullptr));
    setSize(lastUIWidth.getValue(), lastUIHeight.getValue());
    lastUIWidth.addListener(this);
    lastUIHeight.addListener(this);
}

ZLInflatorAudioProcessorEditor::~ZLInflatorAudioProcessorEditor() {
}

//==============================================================================
void ZLInflatorAudioProcessorEditor::paint(juce::Graphics &g) {
}

void ZLInflatorAudioProcessorEditor::resized() {
    mainPanel.setBounds(getLocalBounds());
    lastUIWidth = getWidth();
    lastUIHeight = getHeight();
}

void ZLInflatorAudioProcessorEditor::valueChanged(juce::Value &) {
    setSize(lastUIWidth.getValue(), lastUIHeight.getValue());
}