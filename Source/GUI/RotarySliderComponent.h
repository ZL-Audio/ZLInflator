#ifndef ZLINFLATOR_ROTARYSLIDERCOMPONENT_H
#define ZLINFLATOR_ROTARYSLIDERCOMPONENT_H

#include "NameLookAndFeel.h"
#include "RotarySliderLookAndFeel.h"
#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class RotarySliderComponent : public juce::Component {
public:
    explicit RotarySliderComponent (const juce::String& labelText) : myLookAndFeel() {
        // setup slider
        slider.setSliderStyle (juce::Slider::Rotary);
        slider.setTextBoxIsEditable (false);
        slider.setDoubleClickReturnValue (true, 0.0);
        slider.setLookAndFeel (&myLookAndFeel);
        slider.setScrollWheelEnabled (true);
        addAndMakeVisible (slider);

        // setup label
        label.setText (labelText, juce::dontSendNotification);
        label.setLookAndFeel (&nameLookAndFeel);
        addAndMakeVisible (label);
    }

    ~RotarySliderComponent() override  {
        slider.setLookAndFeel(nullptr);
        label.setLookAndFeel(nullptr);
    }

    void resized() override {
        auto bound = getLocalBounds().toFloat();
        auto boundMinWH = juce::jmin (bound.getWidth(), bound.getHeight() - fontSize * ZLInterface::FontHuge);
        bound = bound.withSizeKeepingCentre (boundMinWH, boundMinWH + fontSize * ZLInterface::FontHuge);
        auto textBound = bound.removeFromTop (fontSize * ZLInterface::FontHuge);
        label.setBounds (textBound.toNearestInt());
        auto bounds = bound;
        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.9f;
        auto buttonBounds = bounds.withSizeKeepingCentre (radius, radius);
        slider.setBounds (buttonBounds.toNearestInt());
    }

    void paint (juce::Graphics& g) override {}

    juce::Slider& getSlider() { return slider; }

    juce::Label& getLabel() { return label; }

    void setFontSize (float size) {
        fontSize = size;
        myLookAndFeel.setFontSize (size);
        nameLookAndFeel.setFontSize (size);
    }

    void setEditable (bool f) {
        myLookAndFeel.setEditable(f);
        nameLookAndFeel.setEditable(f);
        repaint();
    }

private:
    RotarySliderLookAndFeel myLookAndFeel;
    NameLookAndFeel nameLookAndFeel;
    juce::Slider slider;
    juce::Label label;
    float fontSize;

    constexpr static float sliderHeight = 0.85f;
    constexpr static float labelHeight = 1.f - sliderHeight;
};

#endif //ZLINFLATOR_ROTARYSLIDERCOMPONENT_H
