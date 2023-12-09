/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_PLOTPANEL_H
#define ZLINFLATOR_PLOTPANEL_H

#include "../GUI/shaper_plot_component.h"
#include "../DSP/dsp_defines.h"
#include "../PluginProcessor.h"

class PlotPanel : public juce::Component, public juce::AudioProcessorValueTreeState::Listener,
                  private juce::AsyncUpdater {
public:
    explicit PlotPanel(ZLInflatorAudioProcessor &p,
                       zlinterface::UIBase &base);

    ~PlotPanel() override;

    void paint(juce::Graphics &) override;

    void resized() override;

    void parameterChanged(const juce::String &parameterID, float newValue) override;

private:
    ZLInflatorAudioProcessor *processorRef;
    zlinterface::ShaperPlotComponent shaperPlotComponent;
    std::array<juce::String, 6> isPlotChangedParaIDs{zldsp::curve1::ID, zldsp::curve2::ID,
                                                     zldsp::weight::ID, zldsp::autoGain::ID,
                                                     zldsp::style1::ID, zldsp::style2::ID};

    void handleAsyncUpdate() override;
};


#endif //ZLINFLATOR_PLOTPANEL_H
