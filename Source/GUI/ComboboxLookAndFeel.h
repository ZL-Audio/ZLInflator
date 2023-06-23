#ifndef ZLINFLATOR_COMBOBOXLOOKANDFEEL_H
#define ZLINFLATOR_COMBOBOXLOOKANDFEEL_H

#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class ComboboxLookAndFeel : public juce::LookAndFeel_V4 {
public:
    // rounded menu box
    ComboboxLookAndFeel() {
        setColour(juce::PopupMenu::backgroundColourId, ZLInterface::BackgroundInactiveColor);
    }

    void drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown, int, int, int, int,
                      juce::ComboBox &box) override {
        juce::ignoreUnused(isButtonDown);
        auto cornerSize = fontSize * 0.5f;
        auto blurRadius = fontSize * 0.15f;
        if (!box.isPopupActive()) {
            auto boxBounds = juce::Rectangle<float>(0, (float) 0,
                                                    (float) width, (float) height * 1.0f);
            boxBounds = ZLInterface::fillRoundedShadowRectangle(g, boxBounds, cornerSize);
            ZLInterface::fillRoundedInnerShadowRectangle(g, boxBounds, cornerSize, blurRadius,
                                                         true, true, true, true, true);
        } else {
            auto boxBounds = juce::Rectangle<float>(0, 0,
                                                    (float) width * 1.0f, (float) height + cornerSize * 2.f);
            boxBounds = ZLInterface::fillRoundedShadowRectangle(g, boxBounds, cornerSize, true, true, false, false);
            ZLInterface::fillRoundedInnerShadowRectangle(g, boxBounds, cornerSize, blurRadius,
                                                         true, true, true, true, true);

        }
    }

    void positionComboBoxText(juce::ComboBox &box, juce::Label &label) override {
        label.setBounds(0, 0, box.getWidth(), box.getHeight());
    }

    void drawLabel(juce::Graphics &g, juce::Label &label) override {
        if (editable.load()) {
            g.setColour(ZLInterface::TextColor);
        } else {
            g.setColour(ZLInterface::TextInactiveColor);
        }
        auto labelArea = label.getLocalBounds().toFloat();
        auto center = labelArea.getCentre();
        if (fontSize > 0) {
            g.setFont(fontSize * ZLInterface::FontLarge);
        } else {
            g.setFont(labelArea.getHeight() * 0.6f);
        }
        g.drawSingleLineText(juce::String(label.getText()),
                             juce::roundToInt(center.x + g.getCurrentFont().getHorizontalScale()),
                             juce::roundToInt(center.y + g.getCurrentFont().getDescent()),
                             juce::Justification::horizontallyCentred);
    }

    void drawPopupMenuBackground(juce::Graphics &g, int width, int height) override {
        auto cornerSize = fontSize * 0.5f;
        auto blurRadius = fontSize * 0.15f;
        auto boxBounds = juce::Rectangle<float>(0, -2.f * cornerSize, (float) width,
                                                (float) height + 2.f * cornerSize);
        boxBounds = ZLInterface::fillRoundedShadowRectangle(g, boxBounds, cornerSize, false, false, true, true);
        ZLInterface::fillRoundedInnerShadowRectangle(g, boxBounds, cornerSize, blurRadius,
                                                     true, true, true, true, true);
        g.setColour(ZLInterface::TextInactiveColor);
        g.fillRect(boxBounds.getX(), 0.0f, boxBounds.getWidth(), cornerSize * 0.15f);
    }

    void getIdealPopupMenuItemSize(const juce::String &text, const bool isSeparator, int standardMenuItemHeight,
                                   int &idealWidth, int &idealHeight) override {
        juce::ignoreUnused(text, isSeparator, standardMenuItemHeight);
        idealWidth = int(0);
        idealHeight = int(fontSize * ZLInterface::FontLarge * 1.2f);
    }

    void drawPopupMenuItem(juce::Graphics &g, const juce::Rectangle<int> &area,
                           const bool isSeparator, const bool isActive,
                           const bool isHighlighted, const bool isTicked, const bool hasSubMenu,
                           const juce::String &text,
                           const juce::String &shortcutKeyText, const juce::Drawable *icon,
                           const juce::Colour *const textColourToUse) override {
        juce::ignoreUnused(isSeparator, hasSubMenu, shortcutKeyText, icon, textColourToUse);
        if ((isHighlighted || isTicked) && isActive && editable) {
            g.setColour(ZLInterface::TextColor);
        } else {
            g.setColour(ZLInterface::TextInactiveColor);
        }
        if (fontSize > 0) {
            g.setFont(fontSize * ZLInterface::FontLarge);
        } else {
            g.setFont((float) area.getHeight() * 0.35f);
        }
        auto center = area.toFloat().getCentre();
        g.drawSingleLineText(
                text,
                juce::roundToInt(center.x + g.getCurrentFont().getHorizontalScale()),
                juce::roundToInt(center.y),
                juce::Justification::horizontallyCentred);
    }

    int getMenuWindowFlags() override {
        return 1;
    }

    int getPopupMenuBorderSize() override {
        return juce::roundToInt(fontSize * 0.5f);
    }

    void setFontSize(float size) {
        fontSize = size;
    }

    void setEditable(bool f) {
        editable.store(f);
    }

private:
    constexpr static float scale = 1.0f;
    std::atomic<float> fontSize = 0.0f;
    std::atomic<bool> editable = true;
};

#endif //ZLINFLATOR_COMBOBOXLOOKANDFEEL_H
