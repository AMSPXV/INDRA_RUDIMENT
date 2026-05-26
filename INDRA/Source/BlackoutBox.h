#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class BlackoutBox  : public juce::Component
{
public:
    BlackoutBox();
    ~BlackoutBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlackoutBox)
};
