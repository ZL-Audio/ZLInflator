#ifndef ZLINFLATOR_BUTTONCOMPONENT_H
#define ZLINFLATOR_BUTTONCOMPONENT_H

#include "ButtonLookAndFeel.h"
#include "NameLookAndFeel.h"
#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class ButtonComponent : public juce::Component {
public:
    explicit ButtonComponent (const juce::String& labelText) {
        setLookAndFeel (&myLookAndFeel);
        button.setClickingTogglesState (true);
        button.setLookAndFeel (&myLookAndFeel);
        addAndMakeVisible (button);
        label.setText (labelText, juce::dontSendNotification);
        label.setLookAndFeel (&nameLookAndFeel);
        addAndMakeVisible (label);
    }

    ~ButtonComponent() override {
        setLookAndFeel(nullptr);
        button.setLookAndFeel (nullptr);
        label.setLookAndFeel (nullptr);
    }

    void resized() override {
        auto bounds = getLocalBounds();
        bounds.removeFromTop (int (0.1 * bounds.getHeight()));
        button.setBounds (0, (int) (labelHeight * (float) bounds.getHeight()), bounds.getWidth(), (int) ((float) bounds.getWidth() * buttonRatio));
        label.setBoundsRelative (0.f, 0.0f, 1.f, labelHeight);
    }

    void paint (juce::Graphics& g) override {
        g.fillAll (ZLInterface::BackgroundColor);
    }

    juce::ToggleButton& getButton() { return button; }

    juce::Label& getLabel() { return label; }

    void setFontSize (float size) {
        myLookAndFeel.setFontSize (size);
        nameLookAndFeel.setFontSize (size);
    }

    void setEditable (bool f) {
        myLookAndFeel.setEditable(f);
        nameLookAndFeel.setEditable(f);
        repaint();
    }

private:
    ButtonLookAndFeel myLookAndFeel;
    NameLookAndFeel nameLookAndFeel;
    juce::ToggleButton button;
    juce::Label label;

    constexpr static float buttonHeight = 0.7f;
    constexpr static float labelHeight = 1.f - buttonHeight;
    constexpr static float buttonRatio = 0.45f;
};

#endif //ZLINFLATOR_BUTTONCOMPONENT_H
