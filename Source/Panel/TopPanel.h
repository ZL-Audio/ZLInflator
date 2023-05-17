/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_TOPPANEL_H
#define ZLINFLATOR_TOPPANEL_H

#include "../GUI/ComboboxComponent.h"
#include <BinaryData.h>
#include <juce_audio_processors/juce_audio_processors.h>

class TopPanel : public juce::Component {
public:
    explicit TopPanel(juce::AudioProcessorValueTreeState &apvts);

    ~TopPanel() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

    void setFontSize(float size);

private:
    const std::unique_ptr<juce::Drawable> logoDrawable;
    std::unique_ptr<ComboboxComponent> sampleRateCombobox;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboboxAttachments;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};

#endif //ZLINFLATOR_TOPPANEL_H
