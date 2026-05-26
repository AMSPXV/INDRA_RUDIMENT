

#pragma once

#include <JuceHeader.h>
#include "AlbumCoverDisplay.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*

*/

static juce::Font MainSongComponent_FONT{ juce::Typeface::createSystemTypefaceFor(BinaryData::MohaveRegular_ttf,BinaryData::MohaveRegular_ttfSize) };

class MainSongComponent  : public juce::Component
{
public:
    MainSongComponent();
    ~MainSongComponent() override;

    CustomLookAndFeel lookandfeel;
    void paint (juce::Graphics&) override;
    void resized() override;
    void changeArtwork(juce::Image);
    void changeLenght(std::string newLength);
    void changeArtist(std::string newArtist);
    void changeSongName(std::string newSong);
    void changeAlbumName(std::string newAlbum);


private:

    AlbumCoverDisplay artwork;
    juce::Label lenght;
    juce::Label songname;
    juce::Label artistname;
    juce::Label albumname;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainSongComponent)
};
