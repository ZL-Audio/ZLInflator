#ifndef ZLINFLATOR_BUTTONLOOKANDFEEL_H
#define ZLINFLATOR_BUTTONLOOKANDFEEL_H

#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class ButtonLookAndFeel : public juce::LookAndFeel_V4 {
public:
    explicit ButtonLookAndFeel() = default;

    void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override {
        juce::ignoreUnused(shouldDrawButtonAsDown);
        auto bounds = button.getLocalBounds().toFloat();
        // draw button
        bounds = ZLInterface::fillRoundedShadowRectangle(g, bounds, fontSize * 0.5f);
        ZLInterface::fillRoundedInnerShadowRectangle(g, bounds, fontSize * 0.5f, fontSize * 0.15f,
                                                     true, true, true, true,
                                                     true);
        // draw ON/OFF
        if (editable.load()) {
            g.setColour(ZLInterface::TextColor);
        } else {
            g.setColour(ZLInterface::TextInactiveColor);
        }
        if (fontSize > 0) {
            g.setFont(fontSize * ZLInterface::FontLarge);
        } else {
            g.setFont(bounds.getHeight() * 0.35f);
        }
        if (button.getToggleState()) {
            g.drawSingleLineText(juce::String("ON"),
                                 juce::roundToInt(bounds.getCentre().x + bounds.getWidth() * 0.22 +
                                                  g.getCurrentFont().getHorizontalScale() * 0.5f),
                                 juce::roundToInt(bounds.getCentre().y + g.getCurrentFont().getDescent()),
                                 juce::Justification::horizontallyCentred);
        } else {
            g.drawSingleLineText(juce::String("OFF"),
                                 juce::roundToInt(bounds.getCentre().x - bounds.getWidth() * 0.22 +
                                                  g.getCurrentFont().getHorizontalScale() * 0.5f),
                                 juce::roundToInt(bounds.getCentre().y + g.getCurrentFont().getDescent()),
                                 juce::Justification::horizontallyCentred);
        }
        g.setColour(ZLInterface::TextHideColor);
        if (shouldDrawButtonAsHighlighted) {
            if (!button.getToggleState()) {
                g.drawSingleLineText(juce::String("ON"),
                                     juce::roundToInt(bounds.getCentre().x + bounds.getWidth() * 0.22 +
                                                      g.getCurrentFont().getHorizontalScale() * 0.5f),
                                     juce::roundToInt(bounds.getCentre().y + g.getCurrentFont().getDescent()),
                                     juce::Justification::horizontallyCentred);
            } else {
                g.drawSingleLineText(juce::String("OFF"),
                                     juce::roundToInt(bounds.getCentre().x - bounds.getWidth() * 0.22 +
                                                      g.getCurrentFont().getHorizontalScale() * 0.5f),
                                     juce::roundToInt(bounds.getCentre().y + g.getCurrentFont().getDescent()),
                                     juce::Justification::horizontallyCentred);
            }
        }
    }

    void setFontSize(float size) {
        fontSize = size;
    }

    void setEditable(bool f) {
        editable.store(f);
    }

private:
    std::atomic<float> fontSize = 0.0f;
    std::atomic<bool> editable = true;
};

#endif //ZLINFLATOR_BUTTONLOOKANDFEEL_H
