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

    class MeterComponent : public juce::Component, private juce::Timer {
    public:
        explicit MeterComponent(const juce::String &labelText,
                                MeterSource<float> *meterSource,
                                float minV, float maxV,
                                UIBase &base) :
                myLookAndFeel(base), nameLookAndFeel(base) {
            // set meter
            source = meterSource;
            source->setDecayRate(27.f / zlinterface::RefreshFreqHz);
            myLookAndFeel.setRMSRange(minV, maxV);
            setLookAndFeel(&myLookAndFeel);
            startTimerHz(zlinterface::RefreshFreqHz);
            // set label
            label.setText(labelText, juce::dontSendNotification);
            label.setLookAndFeel(&nameLookAndFeel);
            addAndMakeVisible(label);
        }

        ~MeterComponent() override {
            stopTimer();
            setLookAndFeel(nullptr);
            label.setLookAndFeel(nullptr);
        }

        void resized() override {
            label.setBoundsRelative(0.0f, 0.95f, 1.0f, 0.05f);
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
            repaint();
        }

    private:
        MeterSource<float> *source = nullptr;
        juce::Label label;
        MeterLookAndFeel myLookAndFeel;
        NameLookAndFeel nameLookAndFeel;

        UIBase *uiBase;
    };
}

#endif //ZLINFLATOR_METERCOMPONENT_H
