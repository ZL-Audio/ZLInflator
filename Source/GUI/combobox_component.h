// ==============================================================================
// Copyright (C) 2023 - zsliu98
// This file is part of ZLEComp
//
// ZLEComp is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// ZLEComp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with ZLEComp. If not, see <https://www.gnu.org/licenses/>.
// ==============================================================================

#ifndef ZL_COMBOBOX_COMPONENT_H
#define ZL_COMBOBOX_COMPONENT_H

#include "combobox_look_and_feel.h"
#include "name_look_and_feel.h"
#include "interface_definitions.h"
#include "juce_gui_basics/juce_gui_basics.h"


namespace zlinterface {
    class ComboboxComponent : public juce::Component {
    public:
        explicit ComboboxComponent(const juce::String &labelText, const juce::StringArray &choices, UIBase &base) :
                myLookAndFeel(base), nameLookAndFeel(base) {
            uiBase = &base;
            setLookAndFeel(&myLookAndFeel);
            comboBox.addItemList(choices, 1);
            comboBox.setLookAndFeel(&myLookAndFeel);
            comboBox.setScrollWheelEnabled(false);
            addAndMakeVisible(comboBox);
            label.setText(labelText, juce::dontSendNotification);
            label.setLookAndFeel(&nameLookAndFeel);
            addAndMakeVisible(label);

            uiBase = &base;
        }

        ~ComboboxComponent() override {
            setLookAndFeel(nullptr);
            comboBox.setLookAndFeel(nullptr);
            label.setLookAndFeel(nullptr);
        }

        void resized() override {
            auto bound = getLocalBounds().toFloat();
            auto labelBound = bound.removeFromTop(labelHeight * bound.getHeight());
            label.setBounds(labelBound.toNearestInt());
            comboBox.setBounds(bound.toNearestInt());
        }

        void paint(juce::Graphics &g) override {
            juce::ignoreUnused(g);
        }

        juce::ComboBox &getComboBox() { return comboBox; }

        juce::Label &getLabel() { return label; }

        void setEditable(bool f) {
            myLookAndFeel.setEditable(f);
            nameLookAndFeel.setEditable(f);
        }

    private:
        ComboboxLookAndFeel myLookAndFeel;
        NameLookAndFeel nameLookAndFeel;
        juce::ComboBox comboBox;
        juce::Label label;

        constexpr static float boxHeight = 0.7f;
        constexpr static float labelHeight = 1.f - boxHeight;
        constexpr static float boxRatio = 0.45f;

        UIBase *uiBase;
    };
}
#endif //ZL_COMBOBOX_COMPONENT_H
