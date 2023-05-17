/*
==============================================================================
Copyright (C) 2023 - zsliu98
This file is part of ZLInflator

ZLInflator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
ZLInflator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with ZLInflator. If not, see <https://www.gnu.org/licenses/>.
==============================================================================
*/

#include "TopPanel.h"

TopPanel::TopPanel(juce::AudioProcessorValueTreeState & apvts) :
logoDrawable(
        juce::Drawable::createFromImageData(BinaryData::small_logo_svg, BinaryData::small_logo_svgSize)){
    // init combobox
    std::array comboboxList{&sampleRateCombobox};
    std::array comboboxName{"over_sample"};
    for (size_t i = 0; i < comboboxList.size(); ++i) {
        *comboboxList[i] = std::make_unique<ComboboxComponent>(apvts.getParameter(comboboxName[i])->name,
                                                                 apvts.getParameter(comboboxName[i])->getAllValueStrings());
        addAndMakeVisible(*(*comboboxList[i]));
        comboboxAttachments.add(
                std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, comboboxName[i],
                                                                                         (*comboboxList[i])->getComboBox()));
    }
}

TopPanel::~TopPanel() = default;

void TopPanel::paint(juce::Graphics &g) {
    g.fillAll(ZLInterface::BackgroundColor);
    auto oldBounds = getLocalBounds().toFloat();
    auto offset = juce::jmin(oldBounds.getX() + oldBounds.getWidth() * 0.02f, oldBounds.getY() + oldBounds.getHeight() * 0.05f);
    auto bounds = juce::Rectangle<float> (offset, offset, oldBounds.getWidth() * 0.29f, oldBounds.getHeight() * 0.8f);
    auto shiftX = bounds.getX();
    auto shiftY = bounds.getY();
    auto scale = juce::jmin(bounds.getWidth() / (float) logoDrawable->getWidth(), bounds.getHeight() / (float) logoDrawable->getHeight());
    auto transform = juce::AffineTransform::scale(scale).translated(shiftX, shiftY);
    logoDrawable->setTransform(transform);
    logoDrawable->drawAt(g, bounds.getX(), bounds.getY(), 1.0f);
}

void TopPanel::resized() {
    sampleRateCombobox->setBoundsRelative(0.583f, 0.0f, 0.416f, 1.0f);
}

void TopPanel::setFontSize(float size) {
    std::array comboboxList{&sampleRateCombobox};
    for (auto const &c: comboboxList) {
        (*c)->setFontSize(size);
    }
}