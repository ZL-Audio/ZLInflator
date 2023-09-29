/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef ZLINFLATOR_METERPANEL_H
#define ZLINFLATOR_METERPANEL_H

#include "../DSP/MeterSource.h"
#include "../GUI/meter_component.h"
#include "../GUI/interface_definitions.h"

class MeterPanel : public juce::Component {
public:
    explicit MeterPanel(MeterSource<float> *input, MeterSource<float> *output, zlinterface::UIBase &base);

    ~MeterPanel() override;

    void paint(juce::Graphics &) override;

    void resized() override;

private:
    zlinterface::MeterBackgroundComponent inputBackground, outputBackground;
    zlinterface::MeterComponent inputMeter, outputMeter;
};


#endif //ZLINFLATOR_METERPANEL_H
