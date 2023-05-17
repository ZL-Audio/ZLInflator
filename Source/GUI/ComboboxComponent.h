/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_COMBOBOXCOMPONENT_H
#define ZLINFLATOR_COMBOBOXCOMPONENT_H

#include "ComboboxLookAndFeel.h"
#include "NameLookAndFeel.h"
#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class ComboboxComponent : public juce::Component {
public:
    explicit ComboboxComponent (const juce::String &labelText, const juce::StringArray &choices) :
            myLookAndFeel() {
        setLookAndFeel(&myLookAndFeel);
        comboBox.addItemList(choices, 1);
        comboBox.setLookAndFeel(&myLookAndFeel);
        addAndMakeVisible(comboBox);
        label.setText(labelText, juce::dontSendNotification);
        label.setLookAndFeel(&nameLookAndFeel);
        addAndMakeVisible(label);
    }

    void resized() override {
        auto bounds = getLocalBounds();
        bounds.removeFromTop(int (0.1 * bounds.getHeight()));
        comboBox.setBounds(0, (int) (0.3f * (float)bounds.getHeight()), bounds.getWidth(), (int) ((float)bounds.getWidth() * 0.4f));
        label.setBoundsRelative(0.f, 0.0f, 1.f, 0.3f);
    }

    void paint(juce::Graphics &g) override {
        g.fillAll(ZLInterface::BackgroundColor);
    }

    juce::ComboBox &getComboBox() { return comboBox; }

    juce::Label &getLabel() { return label; }

    void setFontSize(float size) {
        myLookAndFeel.setFontSize(size);
        nameLookAndFeel.setFontSize(size);
    }

private:
    ComboboxLookAndFeel myLookAndFeel;
    NameLookAndFeel nameLookAndFeel;
    juce::ComboBox comboBox;
    juce::Label label;
};

#endif //ZLINFLATOR_COMBOBOXCOMPONENT_H
