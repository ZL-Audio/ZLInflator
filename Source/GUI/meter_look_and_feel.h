// ==============================================================================
// Copyright (C) 2023 - zsliu98
// This file is part of ZLEComp
//
// ZLEComp is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// ZLEComp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with ZLEComp. If not, see <https://www.gnu.org/licenses/>.
// ==============================================================================

#ifndef ZLINFLATOR_METERLOOKANDFEEL_H
#define ZLINFLATOR_METERLOOKANDFEEL_H

#include <juce_gui_basics/juce_gui_basics.h>
#include "interface_definitions.h"

namespace zlinterface {

    class MeterLookAndFeel : public juce::LookAndFeel_V4 {
    public:
        explicit MeterLookAndFeel(UIBase &base) {
            uiBase = &base;
        }

        void drawMeters(juce::Graphics &g, const juce::Rectangle<float> &bounds,
                        const std::vector<float> &rms,
                        const std::vector<float> &peak,
                        const std::vector<float> &peakMax) {

            auto bound = bounds.toFloat();
            bound = uiBase->getRoundedShadowRectangleArea(bound,
                                                          uiBase->getFontSize() * .5f,
                                                          {.blurRadius=.25f});
            auto numberBound = bound;

            auto meterWidth = bound.getWidth() / static_cast<float>(rms.size());
            numberBound = numberBound.withTrimmedTop(.05f * meterWidth);
            numberBound = numberBound.withTrimmedBottom(.05f * meterWidth);
            numberBound = numberBound.withTrimmedTop(.08f * numberBound.getHeight());
            drawMeterNumbers(g, numberBound);
            for (size_t i = 0; i < peakMax.size(); ++i) {
                auto localBound = bound.removeFromLeft(meterWidth);
                localBound = localBound.withSizeKeepingCentre(.9f * localBound.getWidth(),
                                                              localBound.getHeight() - 0.1f * localBound.getWidth());
                auto peakBound = localBound.removeFromTop(localBound.getHeight() * 0.08f);
                drawMeterPeakMax(g, peakMax[i], peakBound);
                auto curveTL = false, curveTR = false, curveBL = false, curveBR = false;
                if (i == 0) {
                    curveBL = true;
                } else if (i == peakMax.size() - 1) {
                    curveBR = true;
                }
                g.setColour(uiBase->getTextHideColor());
                drawMeterValue(g, peak[i], localBound, curveTL, curveTR, curveBL, curveBR);

                g.setColour(uiBase->getTextHideColor());
                drawMeterValue(g, rms[i], localBound, curveTL, curveTR, curveBL, curveBR);
            }
        }

        void setRMSRange(float minV, float maxV) {
            minRMS = minV;
            maxRMS = maxV;
        }

    private:
        const static int numNumbers = 5;
        float minRMS = -24.0f, maxRMS = 0.0f;

        UIBase *uiBase;

        void drawMeterPeakMax(juce::Graphics &g, float peakMax, const juce::Rectangle<float> &bound) {
            if (peakMax > 0) {
                g.setColour(uiBase->getTextColor());
            } else {
                g.setColour(uiBase->getTextInactiveColor());
            }
            if (uiBase->getFontSize() > 0) {
                g.setFont(uiBase->getFontSize() * FontSmall);
            } else {
                g.setFont(bound.getHeight() * 0.6f);
            }
            std::string peakMaxDisplay;
            if (peakMax < minRMS) {
                peakMaxDisplay = "-inf";
            } else {
                peakMaxDisplay = formatFloat(peakMax, 1);
            }
            g.drawSingleLineText(peakMaxDisplay,
                                 juce::roundToInt(bound.getCentre().x + g.getCurrentFont().getHorizontalScale()),
                                 juce::roundToInt(bound.getCentre().y + g.getCurrentFont().getDescent()),
                                 juce::Justification::horizontallyCentred);
        }

        void drawMeterValue(juce::Graphics &g, float value, const juce::Rectangle<float> &bound,
                            bool curveTL, bool curveTR, bool curveBL, bool curveBR) {
            value = juce::jlimit(minRMS, maxRMS, value);
            auto scale = (value - minRMS) / (maxRMS - minRMS);
            auto valueBound = bound;
            valueBound = valueBound.withTrimmedTop((1 - scale) * bound.getHeight());

            juce::Path path;
            path.addRoundedRectangle(valueBound.getX(), valueBound.getY(),
                                     valueBound.getWidth(), valueBound.getHeight(),
                                     uiBase->getFontSize() * .5f, uiBase->getFontSize() * .5f,
                                     curveTL, curveTR, curveBL, curveBR);
            g.fillPath(path);
        }

        void drawMeterNumbers(juce::Graphics &g, const juce::Rectangle<float> &bound) {
            auto period = static_cast<float>(static_cast<int>(maxRMS - minRMS)) / numNumbers;
            auto localNumber = maxRMS;
            auto numberBound = bound;
            g.setColour(uiBase->getTextColor());
            if (uiBase->getFontSize() > 0) {
                g.setFont(uiBase->getFontSize() * FontNormal);
            } else {
                g.setFont(bound.getHeight() * 0.6f);
            }
            auto centerX = numberBound.getCentreX();
            for (int i = 0; i < numNumbers; ++i) {
                auto scale = (maxRMS - localNumber) / (maxRMS - minRMS);
                auto centerY = numberBound.getY() + scale * numberBound.getHeight();
                g.drawSingleLineText("-" + formatFloat(std::abs(localNumber), 0) + "-",
                                     juce::roundToInt(centerX + g.getCurrentFont().getHorizontalScale()),
                                     juce::roundToInt(centerY + g.getCurrentFont().getDescent()),
                                     juce::Justification::horizontallyCentred);
                localNumber -= period;
            }
        }
    };
}
#endif //ZLINFLATOR_METERLOOKANDFEEL_H
