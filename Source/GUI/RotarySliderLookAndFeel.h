/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_ROTARYSLIDERLOOKANDFEEL_H
#define ZLINFLATOR_ROTARYSLIDERLOOKANDFEEL_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "interface_defines.h"

class RotarySliderLookAndFeel : public juce::LookAndFeel_V4 {
public:
    explicit RotarySliderLookAndFeel() = default;

    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &slider) override {

        // draw background
        g.fillAll(ZLInterface::BackgroundColor);
        // calculate values
        auto rotationAngle =
                rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto diameter = juce::jmin(bounds.getWidth(), bounds.getHeight());
        bounds = bounds.withSizeKeepingCentre(0.9f * diameter, 0.9f * diameter);
        // draw knob
        ZLInterface::drawEllipse(g, bounds, fontSize * 0.15f);
        // draw arrow
        juce::Path path;
        float triangleUnit = diameter * 0.05f;
        path.addTriangle(-1.15f * triangleUnit, triangleUnit,
                         1.15f * triangleUnit, triangleUnit,
                         0, -triangleUnit);
        auto transform = juce::AffineTransform::translation(
                -0.0f + bounds.getCentreX(), -0.375f * diameter + bounds.getCentreY()).rotated(
                rotationAngle, bounds.getCentreX(), bounds.getCentreY());
        g.setColour(ZLInterface::TextColor);
        g.fillPath(path, transform);
    }

    juce::Label *createSliderTextBox(juce::Slider &) override {
        auto *l = new juce::Label();
        l->setJustificationType(juce::Justification::centred);
        l->setInterceptsMouseClicks(false, false);
        return l;
    }

    juce::Slider::SliderLayout getSliderLayout(juce::Slider &slider) override {
        auto localBounds = slider.getLocalBounds().toFloat();
        juce::Slider::SliderLayout layout;
        auto textBounds = juce::Rectangle<float>(localBounds.getX() + 0.15f * localBounds.getWidth(),
                                                 localBounds.getY() + 0.15f * localBounds.getHeight(),
                                                 localBounds.getWidth() * 0.7f,
                                                 localBounds.getHeight() * 0.7f);
        layout.textBoxBounds = textBounds.toNearestInt();
        layout.sliderBounds = localBounds.toNearestInt();
        return layout;
    }

    void drawLabel(juce::Graphics &g, juce::Label &label) override {
        g.setColour(ZLInterface::TextColor);
        auto labelArea{label.getLocalBounds().toFloat()};
        auto center = labelArea.getCentre();
        if (fontSize > 0) {
            g.setFont(fontSize * ZLInterface::FontHuge);
        } else {
            g.setFont(labelArea.getHeight() * 0.6f);
        }
        g.drawSingleLineText(juce::String(label.getText()),
                             juce::roundToInt(center.x + g.getCurrentFont().getHorizontalScale()),
                             juce::roundToInt(center.y + g.getCurrentFont().getDescent()),
                             juce::Justification::centred);
    }

    void setFontSize(float size) {
        fontSize = size;
    }

private:
    std::atomic<float> fontSize = 0.0f;
};

#endif //ZLINFLATOR_ROTARYSLIDERLOOKANDFEEL_H
