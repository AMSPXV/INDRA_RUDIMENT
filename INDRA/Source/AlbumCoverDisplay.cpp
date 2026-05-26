
#include <JuceHeader.h>
#include "AlbumCoverDisplay.h"

//==============================================================================
AlbumCoverDisplay::AlbumCoverDisplay()
{
    this->albumCover = juce::ImageCache::getFromMemory(BinaryData::DefaultPlay_png, BinaryData::DefaultPlay_pngSize);
    this->cornerRadiusMultiplier = 0.2f;

}

AlbumCoverDisplay::~AlbumCoverDisplay()
{
}

void AlbumCoverDisplay::paint (juce::Graphics& g)
{
    const float cornerRadius = getHeight()/2*(this->cornerRadiusMultiplier);

    juce::Path path;
    path.addRoundedRectangle(getLocalBounds().toFloat(), cornerRadius);
    juce::Image resizedImage = albumCover.rescaled(getLocalBounds().getWidth(), getLocalBounds().getHeight(),juce::Graphics::highResamplingQuality);
    g.reduceClipRegion(path);
    g.drawImageAt(resizedImage, getLocalBounds().getTopLeft().x, getLocalBounds().getTopLeft().y);

}

void AlbumCoverDisplay::resized()
{
   

}


void AlbumCoverDisplay::loadNewImage(juce::Image newImage)
{
    this->albumCover = newImage;
    repaint();
}

void AlbumCoverDisplay::setCornerRadius(float r)
{
    this->cornerRadiusMultiplier = r;
    repaint();
}