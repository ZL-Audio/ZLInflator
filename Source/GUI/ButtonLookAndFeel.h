/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_BUTTONLOOKANDFEEL_H
#define ZLINFLATOR_BUTTONLOOKANDFEEL_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "interface_defines.h"

class ButtonLookAndFeel : public juce::LookAndFeel_V4 {
public:
    explicit ButtonLookAndFeel() = default;

    void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                          bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        // draw background
        g.fillAll(ZLInterface::BackgroundColor);
        // calculate values
        auto bounds = button.getLocalBounds().toFloat();
        bounds = bounds.withSizeKeepingCentre(bounds.getWidth() * 0.8f, bounds.getHeight() * 0.6f);
        // draw button
        ZLInterface::fillRoundedRectangle(g, bounds, fontSize * 0.5f);
        // draw ON/OFF
        g.setColour(ZLInterface::TextColor);
        if (fontSize > 0) {
            g.setFont(fontSize * ZLInterface::FontLarge);
        } else {
            g.setFont(bounds.getHeight() * 0.35f);
        }
        if (button.getToggleState()) {
            g.drawSingleLineText(juce::String("ON"),
                                 juce::roundToInt(bounds.getCentre().x + bounds.getWidth() * 0.22 +
                                     g.getCurrentFont().getHorizontalScale() * 0.5f),
                                 juce::roundToInt(bounds.getCentre().y + g.getCurrentFont().getDescent()),
                                 juce::Justification::centred);
        } else {
            g.drawSingleLineText(juce::String("OFF"),
                                 juce::roundToInt(bounds.getCentre().x - bounds.getWidth() * 0.22 +
                                     g.getCurrentFont().getHorizontalScale() * 0.5f),
                                 juce::roundToInt(bounds.getCentre().y + g.getCurrentFont().getDescent()),
                                 juce::Justification::centred);
        }
        g.setColour(ZLInterface::TextHideColor);
        if (shouldDrawButtonAsHighlighted) {
            if (!button.getToggleState()) {
                g.drawSingleLineText(juce::String("ON"),
                                     juce::roundToInt(bounds.getCentre().x + bounds.getWidth() * 0.22 +
                                         g.getCurrentFont().getHorizontalScale() * 0.5f),
                                     juce::roundToInt(bounds.getCentre().y + g.getCurrentFont().getDescent()),
                                     juce::Justification::centred);
            } else {
                g.drawSingleLineText(juce::String("OFF"),
                                     juce::roundToInt(bounds.getCentre().x - bounds.getWidth() * 0.22 +
                                         g.getCurrentFont().getHorizontalScale() * 0.5f),
                                     juce::roundToInt(bounds.getCentre().y + g.getCurrentFont().getDescent()),
                                     juce::Justification::centred);
            }
        }
    }

    void setFontSize(float size) {
        fontSize = size;
    }

private:
    std::atomic<float> fontSize = 0.0f;
};

#endif //ZLINFLATOR_BUTTONLOOKANDFEEL_H
