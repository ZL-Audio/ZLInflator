/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_COMBOBOXLOOKANDFEEL_H
#define ZLINFLATOR_COMBOBOXLOOKANDFEEL_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "interface_defines.h"

class ComboboxLookAndFeel : public juce::LookAndFeel_V4 {
public:
    // rounded menu box
    ComboboxLookAndFeel() {
        setColour(juce::PopupMenu::backgroundColourId, ZLInterface::BackgroundInactiveColor);
    }

    void drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown,
                      int, int, int, int, juce::ComboBox &box) override {

        float cornerSize = fontSize * 0.5f;
        if (!box.isPopupActive()) {
            auto boxBounds = juce::Rectangle<float>((float) width * 0.1f, (float) height * 0.2f,
                                                    (float) width * 0.8f, (float) height * 0.6f);

            ZLInterface::fillRoundedRectangle(g, boxBounds, cornerSize);
        } else {
            auto boxBounds = juce::Rectangle<float>((float) width * 0.1f, (float) height * 0.2f,
                                                    (float) width * 0.8f, (float) height * 0.8f);
            ZLInterface::fillRoundedRectangle(g, boxBounds, cornerSize,
                                              true, true, false, false);
        }
    }


    void positionComboBoxText(juce::ComboBox &box, juce::Label &label) override {
        label.setBounds(0, 0, box.getWidth(), box.getHeight());
    }

    void drawLabel(juce::Graphics &g, juce::Label &label) override {
        g.setColour(ZLInterface::TextColor);
        auto labelArea = label.getLocalBounds().toFloat();
        auto center = labelArea.getCentre();
        if (fontSize > 0) {
            g.setFont(fontSize * ZLInterface::FontLarge);
        } else {
            g.setFont(labelArea.getHeight() * 0.6f);
        }
        g.drawSingleLineText(juce::String(label.getText()),
                             int(center.x + g.getCurrentFont().getHorizontalScale()),
                             int(center.y + g.getCurrentFont().getDescent()),
                             juce::Justification::centred);
    }

    void drawPopupMenuBackground(juce::Graphics &g, int width, int height) override {
        auto boxBounds = juce::Rectangle<float>((float) width * 0.1f, (float) height * 0.0f,
                                                (float) width * 0.8f, (float) height * 0.98f);
        float cornerSize = fontSize * 0.5f;
        ZLInterface::fillRoundedRectangle(g, boxBounds, cornerSize,
                                          false, false, true, true);
        g.setColour(ZLInterface::TextInactiveColor);
        g.fillRect(boxBounds.getX(), boxBounds.getY(), boxBounds.getWidth(), cornerSize * 0.15f);
    }

    void getIdealPopupMenuItemSize(const juce::String &text, const bool isSeparator,
                                   int standardMenuItemHeight, int &idealWidth, int &idealHeight) override {
        idealHeight = int(fontSize * ZLInterface::FontLarge * 1.2f);
    }

    void drawPopupMenuItem(juce::Graphics &g, const juce::Rectangle<int> &area,
                           const bool isSeparator, const bool isActive,
                           const bool isHighlighted, const bool isTicked,
                           const bool hasSubMenu, const juce::String &text,
                           const juce::String &shortcutKeyText,
                           const juce::Drawable *icon, const juce::Colour *const textColourToUse) override {
        if ((isHighlighted || isTicked) && isActive) {
            g.setColour(ZLInterface::TextColor);
        } else {
            g.setColour(ZLInterface::TextInactiveColor);
        }
        if (fontSize > 0) {
            g.setFont(fontSize * ZLInterface::FontLarge);
        } else {
            g.setFont((float) area.getHeight() * 0.35f);
        }
        g.drawText(text, area, juce::Justification::centred, true);
    }


    int getMenuWindowFlags() override {
        return 1;
    }

    void setFontSize(float size) {
        fontSize = size;
    }

private:
    constexpr static float scale = 1.0f;
    std::atomic<float> fontSize = 0.0f;
};

#endif //ZLINFLATOR_COMBOBOXLOOKANDFEEL_H
