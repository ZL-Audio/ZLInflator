/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_METERCOMPONENT_H
#define ZLINFLATOR_METERCOMPONENT_H

#include "../DSP/MeterSource.h"
#include "MeterLookAndFeel.h"
#include "NameLookAndFeel.h"
#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class MeterComponent : public juce::Component, private juce::Timer {
public:
    explicit MeterComponent (const juce::String &labelText,
        MeterSource<float> *meterSource,
                              float minV, float maxV) :
            myLookAndFeel() {
        // set meter
        source = meterSource;
        source->setDecayRate(ZLInterface::MeterDecay);
        myLookAndFeel.setRMSRange(minV, maxV);
        setLookAndFeel(&myLookAndFeel);
        startTimerHz(ZLInterface::RefreshFreqHz);
        // set label
        label.setText(labelText, juce::dontSendNotification);
        label.setLookAndFeel(&nameLookAndFeel);
        addAndMakeVisible(label);
    }

    ~MeterComponent() override {
        stopTimer();
    }

    void resized() override {
        label.setBoundsRelative(0.0f, 0.95f, 1.0f, 0.05f);
    }

    void paint(juce::Graphics &g) override {
        g.fillAll(ZLInterface::BackgroundColor);
        auto rms = source->getDisplay();
        auto peak = source->getPeak();
        auto peakMax = source->getPeakMax();
        auto bound = getLocalBounds().toFloat();
        bound = bound.withTrimmedBottom(bound.getHeight() * 0.05f);
        myLookAndFeel.drawMeters(g, bound, rms, peak, peakMax);
        source->resetBuffer();
    }

    void mouseDown (const juce::MouseEvent& event) override {
        source->resetPeakMax();
    }

    void setFontSize(float size) {
        myLookAndFeel.setFontSize(size);
        nameLookAndFeel.setFontSize(size);
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
};

#endif //ZLINFLATOR_METERCOMPONENT_H
