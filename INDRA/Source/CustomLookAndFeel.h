
#include <JuceHeader.h>

#pragma once

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
    public:
        CustomLookAndFeel() {

            setDefaultSansSerifTypeface(getCustomFont().getTypeface());
            setColour(juce::Slider::thumbColourId, juce::Colours::white);
            setColour(juce::Slider::backgroundColourId, juce::Colours::black);
            setColour(juce::Slider::ColourIds::trackColourId, juce::Colour::fromRGB(0, 66, 165));
            setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGBA(0, 0, 0, 125));
            setColour(juce::PopupMenu::ColourIds::backgroundColourId, juce::Colours::black);
            setColour(juce::PopupMenu::ColourIds::textColourId, juce::Colour::fromRGB(0, 66, 165));
            setColour(juce::PopupMenu::ColourIds::highlightedTextColourId, juce::Colours::white);
            setColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId, juce::Colour::fromRGB(0, 66, 165));
            setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
            setColour(juce::ListBox::ColourIds::outlineColourId, juce::Colours::transparentBlack);

        }

    static const juce::Font getCustomFont()
    {
        static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::MohaveRegular_ttf, BinaryData::MohaveRegular_ttfSize);
        return juce::Font(typeface);
    }

};