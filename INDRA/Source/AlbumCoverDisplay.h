
#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AlbumCoverDisplay  : public juce::Component
{
public:
    AlbumCoverDisplay();
    ~AlbumCoverDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void loadNewImage(juce::Image newImage);
    void setCornerRadius(float r);

private:

    juce::Image albumCover{juce::ImageCache::getFromMemory(BinaryData::ALBUMPLACEHOLDER_png,BinaryData::ALBUMPLACEHOLDER_pngSize)};
    float cornerRadiusMultiplier;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlbumCoverDisplay)
};
