
#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class playbackSlider  : public juce::Component     
{
public:
    playbackSlider(juce::AudioTransportSource& source);
    ~playbackSlider() override;
       
    void paint (juce::Graphics&) override;
    void resized() override;
    void changeSliderRange(double max);
    void updateSlider(double time);
    juce::Slider slider;

private:

    bool timerUpdate;
    juce::AudioTransportSource* transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (playbackSlider)
};
