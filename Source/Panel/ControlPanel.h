/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_CONTROLPANEL_H
#define ZLINFLATOR_CONTROLPANEL_H

#include "../GUI/ButtonComponent.h"
#include "../GUI/RotarySliderComponent.h"
#include "../GUI/ComboboxComponent.h"
#include "../DSP/dsp_defines.h"
#include <BinaryData.h>
#include <juce_audio_processors/juce_audio_processors.h>

class ControlPanel : public juce::Component {
public:
    explicit ControlPanel(juce::AudioProcessorValueTreeState &apvts);

    ~ControlPanel() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

    void setFontSize(float size);

private:
    std::unique_ptr<RotarySliderComponent> inputGainSlider, outputGainSlider, lowSplitSlider, highSplitSlider, wetSlider, curve1Slider, weightSlider, curve2Slider;
    std::unique_ptr<ButtonComponent> effectButton, splitButton;
    std::unique_ptr<ComboboxComponent> style1Box, style2Box;

    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboboxAttachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlPanel)
};

#endif //ZLINFLATOR_CONTROLPANEL_H
