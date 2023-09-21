// ==============================================================================
// Copyright (C) 2023 - zsliu98
// This file is part of ZLEComp
//
// ZLEComp is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// ZLEComp is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with ZLEComp. If not, see <https://www.gnu.org/licenses/>.
// ==============================================================================

#ifndef ZLECOMP_PROPERTY_H
#define ZLECOMP_PROPERTY_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

namespace zlstate {

    class Property {
    public:
        Property();

        explicit Property(juce::AudioProcessorValueTreeState &apvts);

        void loadAPVTS(juce::AudioProcessorValueTreeState &apvts);

        void saveAPVTS(juce::AudioProcessorValueTreeState &apvts);

    private:
        std::unique_ptr<juce::PropertiesFile> uiFile;

        inline auto static const path = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getFullPathName();
        inline auto static const uiPath = path + "/Audio/Presets/" + JucePlugin_Manufacturer + "/" + JucePlugin_Name + "/ui.xml";
    };

} // zlstate

#endif //ZLECOMP_PROPERTY_H
