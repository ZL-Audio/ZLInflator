// ==============================================================================
// Copyright (C) 2023 - zsliu98
// This file is part of ZLEComp
//
// ZLEComp is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// ZLEComp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with ZLEComp. If not, see <https://www.gnu.org/licenses/>.
// ==============================================================================

#ifndef ZLLMATCH_LOGOPANEL_H
#define ZLLMATCH_LOGOPANEL_H

#include "BinaryData.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "../GUI/interface_definitions.h"
#include "../State/state_definitions.h"
#include "../PluginProcessor.h"

namespace zlpanel {
    class LogoPanel : public juce::Component, private juce::AsyncUpdater {
    public:
        explicit LogoPanel(ZLInflatorAudioProcessor &p,
                           zlinterface::UIBase &base);

        ~LogoPanel() override;

        void paint(juce::Graphics &g) override;

        void mouseDoubleClick(const juce::MouseEvent &event) override;

        void setJustification(int justificationFlags);

    private:
        const std::unique_ptr<juce::Drawable> brandDrawable, logoDrawable;
        zlinterface::UIBase *uiBase;
        ZLInflatorAudioProcessor *processorRef;
        juce::Justification justification = juce::Justification::topLeft;

        void handleAsyncUpdate() override;
    };
}
#endif //ZLLMATCH_LOGOPANEL_H
