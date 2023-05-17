/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#pragma once

#include "GUI/ButtonComponent.h"
#include "GUI/RotarySliderComponent.h"
#include "GUI/interface_defines.h"
#include "Panel/MainPanel.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ZLInflatorAudioProcessorEditor : public juce::AudioProcessorEditor,
                                       private juce::Value::Listener{
public:
    explicit ZLInflatorAudioProcessorEditor(ZLInflatorAudioProcessor &);

    ~ZLInflatorAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

private:
    ZLInflatorAudioProcessor &audioProcessor;
    MainPanel mainPanel;
    juce::Value lastUIWidth, lastUIHeight;

    void valueChanged(juce::Value &) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZLInflatorAudioProcessorEditor)
};
