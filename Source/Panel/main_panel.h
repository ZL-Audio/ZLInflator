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
#include "../GUI/interface_definitions.h"
#include "control_panel.h"
#include "top_panel.h"
#include "meter_panel.h"
#include "plot_panel.h"

class MainPanel : public juce::Component {
public:
    explicit MainPanel(ZLInflatorAudioProcessor &p);

    ~MainPanel() override;

    void paint(juce::Graphics &) override;

    void resized() override;
private:
    zlinterface::UIBase uiBase;

    ControlPanel controlPanel;
    TopPanel topPanel;
    MeterPanel meterPanel;
    PlotPanel plotPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};

#endif //ZLINFLATOR_MAINPANEL_H
