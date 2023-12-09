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
#include "interface_definitions.h"
#include "../DSP/ShaperFunctions.h"

namespace zlinterface {
    class ShaperPlotComponent : public juce::Component {
    public:
        explicit ShaperPlotComponent(shaper::ShaperMixer<float> *shaperMixer,
                                     zlinterface::UIBase &base) {
            shaper = shaperMixer;
            uiBase = &base;
        }

        ~ShaperPlotComponent() override = default;

        void paint(juce::Graphics &g) override {
            auto thickNess = 0.1f * uiBase->getFontSize();

            g.fillAll(uiBase->getBackgroundColor());
            g.setColour(uiBase->getTextInactiveColor());
            g.drawRect(getLocalBounds().toFloat(), thickNess);

            auto bound = getLocalBounds().toFloat();

            juce::Path path0;
            path0.startNewSubPath(getPointX(xMin, bound), getPointY(yMin, bound));
            path0.lineTo(getPointX(xMax, bound), getPointY(yMax, bound));
            g.setColour(uiBase->getTextHideColor());
            g.strokePath(path0, juce::PathStrokeType(thickNess, juce::PathStrokeType::curved));

            juce::Path path;
            path.startNewSubPath(getPointX(xMin, bound), getPointY(yMin, bound));
            for (int i = static_cast<int>(bound.getX()); i < static_cast<int>(bound.getX() + bound.getWidth()); ++i) {
                auto x = getValueX(static_cast<float>(i), bound);
                auto y = (*shaper)(juce::Decibels::decibelsToGain(x));
                y = juce::Decibels::gainToDecibels(y);
                path.lineTo(static_cast<float>(i), getPointY(y, bound));
            }
            g.setColour(uiBase->getTextColor());
            g.strokePath(path, juce::PathStrokeType(thickNess, juce::PathStrokeType::curved));
        }

    private:
        shaper::ShaperMixer<float> *shaper;
        UIBase *uiBase;
        auto static constexpr xMin = -45.f, xMax = 0.f;
        auto static constexpr yMin = -45.f, yMax = 0.f;

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
}
#endif //ZLINFLATOR_SHAPERPLOTCOMPONENT_H
