
#include <JuceHeader.h>
#include "BlackoutBox.h"

//==============================================================================
BlackoutBox::BlackoutBox()
{


}

BlackoutBox::~BlackoutBox()
{
}

void BlackoutBox::paint (juce::Graphics& g)
{
    g.setColour(juce::Colour::fromRGBA(0, 0, 0, 200));
    g.fillRect(getLocalBounds());

}

void BlackoutBox::resized()
{


}
