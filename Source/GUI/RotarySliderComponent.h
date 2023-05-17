/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_ROTARYSLIDERCOMPONENT_H
#define ZLINFLATOR_ROTARYSLIDERCOMPONENT_H

#include "NameLookAndFeel.h"
#include "RotarySliderLookAndFeel.h"
#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class RotarySliderComponent : public juce::Component {
public:

    explicit RotarySliderComponent(const juce::String &labelText) :
            myLookAndFeel() {
        // setup slider
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxIsEditable(false);
        slider.setDoubleClickReturnValue(true, 0.0);
        slider.setLookAndFeel(&myLookAndFeel);
        addAndMakeVisible(slider);

        // setup label
        label.setText(labelText, juce::dontSendNotification);
        label.setLookAndFeel(&nameLookAndFeel);
        addAndMakeVisible(label);
    }

    void resized() override {
        auto bound = getLocalBounds().toFloat();
        auto boundMinWH = juce::jmin(bound.getWidth(), bound.getHeight() - fontSize * ZLInterface::FontHuge);
        bound = bound.withSizeKeepingCentre(boundMinWH, boundMinWH + fontSize * ZLInterface::FontHuge);
        auto textBound = bound.removeFromTop(fontSize * ZLInterface::FontHuge);
        label.setBounds(textBound.toNearestInt());
        auto bounds = bound;
        auto radius = juce::jmin(sliderHeight * bounds.getWidth(), sliderHeight * bounds.getHeight());
        auto buttonBounds = bounds.withSizeKeepingCentre(radius, radius);
        slider.setBounds(buttonBounds.toNearestInt());
    }

    void paint(juce::Graphics &g) override {
        g.fillAll(ZLInterface::BackgroundColor);
    }

    juce::Slider &getSlider() { return slider; }

    juce::Label &getLabel() { return label; }

    void setFontSize(float size) {
        fontSize = size;
        myLookAndFeel.setFontSize(size);
        nameLookAndFeel.setFontSize(size);
    }

private:
    RotarySliderLookAndFeel myLookAndFeel;
    NameLookAndFeel nameLookAndFeel;
    juce::Slider slider;
    juce::Label label;
    float fontSize;

    constexpr static float sliderHeight = 0.85f;
    constexpr static float labelHeight = 1.f - sliderHeight;
};

#endif //ZLINFLATOR_ROTARYSLIDERCOMPONENT_H
