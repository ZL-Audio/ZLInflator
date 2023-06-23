#ifndef ZLINFLATOR_COMBOBOXCOMPONENT_H
#define ZLINFLATOR_COMBOBOXCOMPONENT_H

#include "ComboboxLookAndFeel.h"
#include "NameLookAndFeel.h"
#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class ComboboxComponent : public juce::Component {
public:
    explicit ComboboxComponent (const juce::String& labelText, const juce::StringArray& choices) : myLookAndFeel() {
        setLookAndFeel (&myLookAndFeel);
        comboBox.addItemList (choices, 1);
        comboBox.setLookAndFeel (&myLookAndFeel);
        comboBox.setScrollWheelEnabled (false);
        addAndMakeVisible (comboBox);
        label.setText (labelText, juce::dontSendNotification);
        label.setLookAndFeel (&nameLookAndFeel);
        addAndMakeVisible (label);
    }

    ~ComboboxComponent() override {
        setLookAndFeel(nullptr);
        comboBox.setLookAndFeel (nullptr);
        label.setLookAndFeel (nullptr);
    }

    void resized() override {
        auto bounds = getLocalBounds();
        bounds.removeFromTop (int (0.1 * bounds.getHeight()));
        comboBox.setBounds (0, (int) (0.3f * (float) bounds.getHeight()), bounds.getWidth(), (int) ((float) bounds.getWidth() * 0.45f));
        label.setBoundsRelative (0.f, 0.0f, 1.f, 0.3f);
    }

    void paint (juce::Graphics& g) override {
        g.fillAll (ZLInterface::BackgroundColor);
    }

    juce::ComboBox& getComboBox() { return comboBox; }

    juce::Label& getLabel() { return label; }

    void setFontSize (float size) {
        myLookAndFeel.setFontSize (size);
        nameLookAndFeel.setFontSize (size);
    }

    void setEditable (bool f) {
        myLookAndFeel.setEditable (f);
        nameLookAndFeel.setEditable (f);
        repaint();
    }

private:
    ComboboxLookAndFeel myLookAndFeel;
    NameLookAndFeel nameLookAndFeel;
    juce::ComboBox comboBox;
    juce::Label label;
};

#endif //ZLINFLATOR_COMBOBOXCOMPONENT_H
