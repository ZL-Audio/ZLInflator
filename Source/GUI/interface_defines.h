/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_INTERFACE_DEFINES_H
#define ZLINFLATOR_INTERFACE_DEFINES_H

#include "juce_gui_basics/juce_gui_basics.h"

namespace ZLInterface {
    auto static const WindowHeight = 320;
    auto static const WindowWidth = 720;
    auto static const WindowFixedAspectRatio = 2.25;
    auto static const WindowMinHeight = 160;
    auto static const WindowMinWidth = 360;
    auto static const WindowMaxHeight = WindowMinHeight * 10;
    auto static const WindowMaxWidth =  WindowMinWidth * 10;


    auto static const TextColor = juce::Colour(87, 96, 110);
    auto static const TextInactiveColor = juce::Colour(87, 96, 110).withAlpha(0.5f);
    auto static const TextHideColor = juce::Colour(87, 96, 110).withAlpha(0.25f);
    auto static const BackgroundColor = juce::Colour(214, 223, 236);
    auto static const BackgroundInactiveColor = juce::Colour(214, 223, 236).withAlpha(0.8f);
    auto static const BackgroundHideColor = juce::Colour(214, 223, 236).withAlpha(0.5f);
    auto static const DarkShadowColor = juce::Colour(119, 122, 126);
    auto static const BrightShadowColor = juce::Colour(232, 241, 250);

    auto static const FontTiny = 0.5f;
    auto static const FontSmall = 0.75f;
    auto static const FontNormal = 1.0f;
    auto static const FontLarge = 1.25f;
    auto static const FontHuge = 1.5f;

    auto static const RefreshFreqHz = 120;
    auto static const MeterDecay = 9.0f / RefreshFreqHz;

    static void fillRoundedRectangle(juce::Graphics &g, juce::Rectangle<float> boxBounds, float cornerSize,
                                     bool curveTopLeft = true, bool curveTopRight = true,
                                     bool curveBottomLeft = true, bool curveBottomRight = true) {
        juce::Path path;
        path.addRoundedRectangle(boxBounds.getX(), boxBounds.getY(),
                                 boxBounds.getWidth(), boxBounds.getHeight(),
                                 cornerSize, cornerSize,
                                 curveTopLeft, curveTopRight,
                                 curveBottomLeft, curveBottomRight);
        juce::DropShadow darkShadow(ZLInterface::DarkShadowColor, 1,
                                    {juce::roundToInt(cornerSize * 0.25f), juce::roundToInt(cornerSize * 0.25f)});
        darkShadow.drawForPath(g, path);
        juce::DropShadow brightShadow(ZLInterface::BrightShadowColor, 1,
                                      {-juce::roundToInt(cornerSize * 0.25f), -juce::roundToInt(cornerSize * 0.25f)});
        brightShadow.drawForPath(g, path);
        g.setColour(ZLInterface::BackgroundColor);
        g.fillPath(path);
    }

    static void drawEllipse(juce::Graphics &g, juce::Rectangle<float> boxBounds, float shadowDisplace) {
        juce::Path path;
        path.addEllipse(boxBounds);
        juce::DropShadow darkShadow(ZLInterface::DarkShadowColor, 1,
                                    {juce::roundToInt(shadowDisplace), juce::roundToInt(shadowDisplace)});
        darkShadow.drawForPath(g, path);
        juce::DropShadow brightShadow(ZLInterface::BrightShadowColor, 1,
                                      {-juce::roundToInt(shadowDisplace), -juce::roundToInt(shadowDisplace)});
        brightShadow.drawForPath(g, path);
        g.setColour(ZLInterface::BackgroundColor);
        g.fillPath(path);
    }

    static std::string formatFloat(float x, int precision) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << x;
        return stream.str();
    }
}

#endif //ZLINFLATOR_INTERFACE_DEFINES_H
