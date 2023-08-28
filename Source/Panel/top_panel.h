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

#include "../GUI/combobox_component.h"
#include "logo_panel.h"
#include "panel_definitions.h"
#include <BinaryData.h>
#include <juce_audio_processors/juce_audio_processors.h>

class TopPanel : public juce::Component {
public:
    explicit TopPanel(ZLInflatorAudioProcessor &p,
                      zlinterface::UIBase &base);

    ~TopPanel() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

private:
    std::unique_ptr<zlinterface::ComboboxComponent> sampleRateCombobox;
    std::array<std::unique_ptr<zlinterface::ComboboxComponent> *, 1> comboBoxList{&sampleRateCombobox};
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboboxAttachments;

    zlinterface::UIBase *uiBase;
    zlpanel::LogoPanel logoPanel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};

#endif //ZLINFLATOR_TOPPANEL_H
