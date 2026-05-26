
#include <JuceHeader.h>
#include "playbackSlider.h"

//==============================================================================
playbackSlider::playbackSlider(juce::AudioTransportSource& source)
{
    transportSource = &source;
    slider.setBounds(getLocalBounds());
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(slider);

}

playbackSlider::~playbackSlider()
{

}

void playbackSlider::paint (juce::Graphics& g)
{

}

void playbackSlider::resized()
{
    slider.setBounds(getLocalBounds());
}


void playbackSlider::changeSliderRange(double max)
{
    slider.setRange(0, max);
}

void playbackSlider:: updateSlider(double time)
{
    slider.setValue(time);

}