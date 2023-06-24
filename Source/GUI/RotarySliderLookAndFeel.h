#ifndef ZLINFLATOR_ROTARYSLIDERLOOKANDFEEL_H
#define ZLINFLATOR_ROTARYSLIDERLOOKANDFEEL_H

#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class RotarySliderLookAndFeel : public juce::LookAndFeel_V4 {
public:
    explicit RotarySliderLookAndFeel() = default;

    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &slider) override {
        juce::ignoreUnused(slider);
        // calculate values
        auto rotationAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto diameter = juce::jmin(bounds.getWidth(), bounds.getHeight());
        bounds = bounds.withSizeKeepingCentre(diameter, diameter);
        // draw knob
        auto oldBounds = ZLInterface::drawInnerShadowEllipse(g, bounds, fontSize * 0.5f);
        auto newBounds = ZLInterface::drawShadowEllipse(g, oldBounds, fontSize * 0.5f);
        ZLInterface::drawInnerShadowEllipse(g, newBounds, fontSize * 0.15f, true);
        // draw arrow
        auto arrowUnit = (diameter - newBounds.getWidth()) * 0.5f;
        auto arrowBound = juce::Rectangle<float>(
                -0.5f * arrowUnit + bounds.getCentreX() +
                (0.5f * diameter - 0.5f * arrowUnit) * std::sin(rotationAngle),
                -0.5f * arrowUnit + bounds.getCentreY() +
                (0.5f * diameter - 0.5f * arrowUnit) * (-std::cos(rotationAngle)),
                arrowUnit, arrowUnit);
        auto arrowStartBound = juce::Rectangle<float>(
                -0.5f * arrowUnit + bounds.getCentreX() +
                (0.5f * diameter - 0.5f * arrowUnit) * std::sin(rotaryStartAngle),
                -0.5f * arrowUnit + bounds.getCentreY() +
                (0.5f * diameter - 0.5f * arrowUnit) * (-std::cos(rotaryStartAngle)),
                arrowUnit, arrowUnit);
        juce::Path mask;
        mask.addEllipse(bounds);
        mask.setUsingNonZeroWinding(false);
        mask.addEllipse(newBounds);
        g.saveState();
        g.reduceClipRegion(mask);
        ZLInterface::drawShadowEllipse(g, arrowBound, fontSize * 0.5f,
                                       ZLInterface::BackgroundColor,
                                       false, false, true);
        ZLInterface::drawShadowEllipse(g, arrowStartBound, fontSize * 0.5f,
                                       ZLInterface::TextHideColor,
                                       false, false, true);

        juce::Path filling;
        filling.addPieSegment(bounds, rotaryStartAngle, rotationAngle, 0);
        filling.setUsingNonZeroWinding(false);
        filling.addPieSegment(arrowStartBound, rotaryStartAngle, rotaryStartAngle + juce::MathConstants<float>::pi,
                              0);
        g.setColour(ZLInterface::TextHideColor);
        g.fillPath(filling);
        ZLInterface::drawInnerShadowEllipse(g, arrowBound, fontSize * 0.15f, true);
        g.restoreState();
    }

    juce::Label *createSliderTextBox(juce::Slider &) override {
        auto *l = new juce::Label();
        l->setJustificationType(juce::Justification::centred);
        l->setInterceptsMouseClicks(false, false);
        return l;
    }

    juce::Slider::SliderLayout getSliderLayout(juce::Slider &slider) override {
        auto localBounds = slider.getLocalBounds().toFloat();
        juce::Slider::SliderLayout layout;
        auto textBounds = juce::Rectangle<float>(localBounds.getX() + 0.15f * localBounds.getWidth(),
                                                 localBounds.getY() + 0.15f * localBounds.getHeight(),
                                                 localBounds.getWidth() * 0.7f,
                                                 localBounds.getHeight() * 0.7f);
        layout.textBoxBounds = textBounds.toNearestInt();
        layout.sliderBounds = slider.getLocalBounds();
        return layout;
    }

    void drawLabel(juce::Graphics &g, juce::Label &label) override {
        if (editable.load()) {
            g.setColour(ZLInterface::TextColor);
        } else {
            g.setColour(ZLInterface::TextInactiveColor);
        }
        auto labelArea{label.getLocalBounds().toFloat()};
        auto center = labelArea.getCentre();
        if (fontSize > 0) {
            g.setFont(fontSize * ZLInterface::FontHuge);
        } else {
            g.setFont(labelArea.getHeight() * 0.6f);
        }
        g.drawSingleLineText(juce::String(label.getText()),
                             juce::roundToInt(center.x + g.getCurrentFont().getHorizontalScale()),
                             juce::roundToInt(center.y + g.getCurrentFont().getDescent()),
                             juce::Justification::horizontallyCentred);
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

#endif //ZLINFLATOR_ROTARYSLIDERLOOKANDFEEL_H
