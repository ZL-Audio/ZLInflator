/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_SHAPERPLOTCOMPONENT_H
#define ZLINFLATOR_SHAPERPLOTCOMPONENT_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "interface_defines.h"
#include "../DSP/ShaperFunctions.h"

class ShaperPlotComponent : public juce::Component, private juce::Timer {
public:
    explicit ShaperPlotComponent(shaper::ShaperMixer<float> *shaperMixer) {
        shaper = shaperMixer;
        startTimerHz(ZLInterface::RefreshFreqHz);
    }

    ~ShaperPlotComponent() override {
        stopTimer();
    }

    void paint(juce::Graphics &g) override {
        g.fillAll(ZLInterface::BackgroundColor);
        g.setColour(ZLInterface::TextInactiveColor);
        g.drawRect(getLocalBounds().toFloat(), thickNess);

        auto bound = getLocalBounds().toFloat();

        juce::Path path0;
        path0.startNewSubPath(getPointX(0, bound), getPointY(0, bound));
        path0.lineTo(getPointX(1, bound), getPointY(1, bound));
        g.setColour(ZLInterface::TextHideColor);
        g.strokePath(path0, juce::PathStrokeType(thickNess, juce::PathStrokeType::curved));


        juce::Path path;
        path.startNewSubPath(getPointX(0, bound), getPointY(0, bound));
        for (int i = static_cast<int>(bound.getX()); i < static_cast<int>(bound.getX() + bound.getWidth()); ++i) {
            auto x = getValueX(static_cast<float>(i), bound);
            auto y = (*shaper)(x);
            path.lineTo( static_cast<float>(i), getPointY(y, bound));
        }
        g.setColour(ZLInterface::TextColor);
        g.strokePath(path, juce::PathStrokeType(thickNess, juce::PathStrokeType::curved));
    }

    void setFontSize(float size) {
        thickNess = size * 0.1f;
    }

    void timerCallback() override {
        if (shaper->hasNewFunction()) {
            repaint();
        }
    }

private:
    shaper::ShaperMixer<float> *shaper;
    float thickNess = 0.0f;
    auto static constexpr xMin = 0.0f, xMax = 1.0f;
    auto static constexpr yMin = 0.0f, yMax = 1.0f;

    static float getValueX(float posX, juce::Rectangle<float> bound) {
        return (posX - bound.getX()) / bound.getWidth() * (xMax - xMin) + xMin;
    }

    static float getPointX(float x, juce::Rectangle<float> bound) {
        return bound.getX() + (x - xMin) / (xMax - xMin) * bound.getWidth();
    }

    static float getPointY(float y, juce::Rectangle<float> bound) {
        return bound.getY() + (1 - (y - yMin) / (yMax - yMin)) * bound.getHeight();
    }
};

#endif //ZLINFLATOR_SHAPERPLOTCOMPONENT_H
