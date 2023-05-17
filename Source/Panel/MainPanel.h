/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_MAINPANEL_H
#define ZLINFLATOR_MAINPANEL_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "../GUI/interface_defines.h"
#include "ControlPanel.h"
#include "TopPanel.h"
#include "MeterPanel.h"
#include "PlotPanel.h"

class MainPanel : public juce::Component {
public:
    explicit MainPanel(juce::AudioProcessorValueTreeState &apvts,
        MeterSource<float> *input,
        MeterSource<float> *output,
        shaper::ShaperMixer<float> *shaperMixer);

    ~MainPanel() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

    void setFontSize(float size);

private:
    ControlPanel controlPanel;
    TopPanel topPanel;
    MeterPanel meterPanel;
    PlotPanel plotPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};

#endif //ZLINFLATOR_MAINPANEL_H
