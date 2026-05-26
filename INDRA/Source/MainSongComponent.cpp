

#include <JuceHeader.h>
#include "MainSongComponent.h"

//==============================================================================
MainSongComponent::MainSongComponent()
{

    //ARTWORK
    artwork.setCornerRadius(0.30f);
    addAndMakeVisible(artwork);

    //LENGHT
    lenght.setColour(juce::Label::textColourId, juce::Colour::fromRGBA(0, 66, 165,255));
    lenght.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lenght);
    lenght.setText(" ? : ? ? ", juce::dontSendNotification);

    //ARTIST NAME
    artistname.setColour(juce::Label::textColourId, juce::Colour::fromRGBA(0, 66, 165, 255));
    artistname.setJustificationType(juce::Justification::centredLeft);
    artistname.setText("Unknown", juce::dontSendNotification);
    addAndMakeVisible(artistname);

    //SONG NAME
    songname.setColour(juce::Label::textColourId, juce::Colour::fromRGBA(255, 255, 255, 255));
    songname.setJustificationType(juce::Justification::centredLeft);
    songname.setText("Unknown", juce::dontSendNotification);
    addAndMakeVisible(songname);

    // ALBUM NAME

    albumname.setColour(juce::Label::textColourId, juce::Colour::fromRGBA(0, 66, 165, 255));
    albumname.setJustificationType(juce::Justification::centredLeft);
    albumname.setText("Unknown", juce::dontSendNotification);
    addAndMakeVisible(albumname);

}

MainSongComponent::~MainSongComponent()
{
}

void MainSongComponent::paint (juce::Graphics& g)
{
    auto localbounds = getLocalBounds();
    auto center = localbounds.getCentre().toFloat();
    auto topleft = localbounds.getTopLeft().toFloat();
    auto height = getHeight();
    auto width = getWidth();
    float cornerRadius = height / 2 * 0.3;
    auto highlightRect = juce::Rectangle<float>(topleft.x + (width / 50), topleft.y + (height / 12), width * 0.96, height * 0.3);
    auto highlightRect_center = highlightRect.getCentre();
    auto highlightRect_topleft = highlightRect.getTopLeft();
    auto highlightRect_height = highlightRect.getHeight();

    juce::Path path;
    path.addRoundedRectangle(localbounds.toFloat(), cornerRadius);
    g.reduceClipRegion(path);
    g.setGradientFill(juce::ColourGradient(juce::Colour(38, 38, 38), center.x, topleft.y, juce::Colour(3, 3, 3), center.x, center.y - height/4, false));
    g.fillAll();
    path.clear();
    path.addRoundedRectangle(highlightRect, highlightRect_height / 2);
    g.setGradientFill(juce::ColourGradient(juce::Colour::fromRGBA(255,255,255,60), highlightRect_center.x, highlightRect_topleft.y, juce::Colours::transparentWhite, highlightRect_center.x,highlightRect.getBottomLeft().y, false));
    g.fillPath(path);

}

void MainSongComponent::resized()
{
    auto localbounds = getLocalBounds();
    auto topleft = localbounds.getTopLeft().toFloat();
    auto bottomright = localbounds.getBottomRight().toFloat();
    auto artworkSize = bottomright.y - (bottomright.y / 12 * 2);

    // ARTWORK

    artwork.setBounds(topleft.x + (bottomright.x / 50), topleft.y + (bottomright.y / 12), artworkSize, artworkSize);

    // LENGTH

    auto lenghtSize = artworkSize;
    lenght.setBounds(bottomright.x-lenghtSize,bottomright.y-(bottomright.y / 4), lenghtSize, bottomright.y / 4);
    lenght.setFont(juce::Font(bottomright.y / 4));

    // ARTIST NAME

    auto artistNameWidth = bottomright.x / 8;
    auto artistHeight = bottomright.y / 4;
    artistname.setBounds(artistNameWidth, bottomright.y / 7 * 3, artistNameWidth*3, artistHeight);
    artistname.setFont(artistHeight);

    // SONG NAME
    songname.setBounds(artistNameWidth, bottomright.y / 7 * 1.1,artistNameWidth*3, artistHeight*1.5);
    songname.setFont(artistHeight * 1.5);

    // ALBUM NAME

    albumname.setBounds(artistNameWidth, bottomright.y / 7 * 4.5, artistNameWidth * 3, artistHeight);
    albumname.setFont(artistHeight);


}

void MainSongComponent::changeArtwork(juce::Image lol) {

    this->artwork.loadNewImage(lol);

}

void MainSongComponent::changeLenght(std::string newLength) {

    lenght.setText(newLength,juce::NotificationType::dontSendNotification);

}

void MainSongComponent::changeArtist(std::string newArtist) {

    artistname.setText(newArtist, juce::NotificationType::dontSendNotification);

}

void MainSongComponent::changeSongName(std::string newSong) {

    songname.setText(newSong, juce::NotificationType::dontSendNotification);

}

void MainSongComponent::changeAlbumName(std::string newAlbum) {

    albumname.setText(newAlbum, juce::NotificationType::dontSendNotification);

}