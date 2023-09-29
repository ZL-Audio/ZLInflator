// ==============================================================================
// Copyright (C) 2023 - zsliu98
// This file is part of ZLEComp
//
// ZLEComp is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// ZLEComp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with ZLEComp. If not, see <https://www.gnu.org/licenses/>.
// ==============================================================================

#ifndef ZLINFLATOR_METERCOMPONENT_H
#define ZLINFLATOR_METERCOMPONENT_H

#include <juce_gui_basics/juce_gui_basics.h>
#include "../DSP/MeterSource.h"
#include "meter_look_and_feel.h"
#include "name_look_and_feel.h"
#include "interface_definitions.h"

namespace zlinterface {

    class MeterBackgroundComponent : public juce::Component {
    public:
        explicit MeterBackgroundComponent(const juce::String &labelText, UIBase &base) :
                nameLookAndFeel(base) {
            uiBase = &base;
            label.setText(labelText, juce::dontSendNotification);
            label.setLookAndFeel(&nameLookAndFeel);
            addAndMakeVisible(label);
        }

        ~MeterBackgroundComponent() override {
            setLookAndFeel(nullptr);
            label.setLookAndFeel(nullptr);
        }

        void paint(juce::Graphics &g) override {
            auto bound = getLocalBounds().toFloat();
            bound = bound.withTrimmedBottom(bound.getHeight() * 0.05f);
            bound = uiBase->fillRoundedShadowRectangle(g, bound,
                                                       uiBase->getFontSize() * .5f,
                                                       {.blurRadius=.25f});
            uiBase->fillRoundedInnerShadowRectangle(g, bound,
                                                    uiBase->getFontSize() * .5f,
                                                    {.blurRadius=.25f, .flip=true});
        }

        void resized() override {
            label.setBoundsRelative(0.0f, 0.95f, 1.0f, 0.05f);
        }

    private:
        UIBase *uiBase;
        juce::Label label;
        NameLookAndFeel nameLookAndFeel;
    };

    class MeterComponent : public juce::Component, private juce::Timer, private juce::AsyncUpdater {
    public:
        explicit MeterComponent(MeterSource<float> *meterSource,
                                float minV, float maxV,
                                UIBase &base) :
                myLookAndFeel(base), nameLookAndFeel(base) {
            uiBase = &base;
            // set meter
            source = meterSource;
            source->setDecayRate(27.f / zlinterface::RefreshFreqHz);
            myLookAndFeel.setRMSRange(minV, maxV);
            setLookAndFeel(&myLookAndFeel);
            startTimerHz(zlinterface::RefreshFreqHz);
        }

        ~MeterComponent() override {
            stopTimer();
            setLookAndFeel(nullptr);
        }

        void paint(juce::Graphics &g) override {
            auto rms = source->getDisplayRMS();
            auto peak = source->getDisplayPeak();
            auto peakMax = source->getPeakMax();
            auto bound = getLocalBounds().toFloat();
            bound = bound.withTrimmedBottom(bound.getHeight() * 0.05f);
            myLookAndFeel.drawMeters(g, bound, rms, peak, peakMax);
            source->resetBuffer();
        }

        void mouseDown(const juce::MouseEvent &event) override {
            juce::ignoreUnused(event);
            source->resetPeakMax();
        }

        void setRMSRange(float minV, float maxV) {
            myLookAndFeel.setRMSRange(minV, maxV);
        }

        void timerCallback() override {
            triggerAsyncUpdate();
        }

    private:
        MeterSource<float> *source = nullptr;
        MeterLookAndFeel myLookAndFeel;
        NameLookAndFeel nameLookAndFeel;

        UIBase *uiBase;

        void handleAsyncUpdate() override {
            repaint();
        }
    };
}

#endif //ZLINFLATOR_METERCOMPONENT_H
