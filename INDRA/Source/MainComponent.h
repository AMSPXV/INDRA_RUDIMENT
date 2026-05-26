
#pragma once

#include <JuceHeader.h>
#include "playbackSlider.h"
#include "AlbumCoverDisplay.h"
#include "MainSongComponent.h"
#include "CustomLookAndFeel.h"
#include "BlackoutBox.h"
#include "UsersSelect.h"
#include "Mp3BoxModel.h"
#include "SideBarListModel.h"

//==============================================================================
/*
*/
class MainComponent : 
                      public juce::ChangeListener,
                      public juce::AudioAppComponent,
                      public juce::Timer
{
public:
    MainComponent(int user = 1);
    ~MainComponent() override;
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void timerCallback() override;
    void releaseResources() override;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    };

    enum LoopState
    {
        QueueLooped,
        SongLooped,
        NoLoop
    };

    void changeState(TransportState newState);
    void openButtonClicked();
    void playButtonClicked();
    void OPENbuttonClicked();
    void DirectoryButtonClicked();
    void usersButtonClicked();
    void blackoutBoxToggle();
    void loopButtonClicked();
    void tabButtonClicked(int button);
    void updateSongInfoLabels(songInfo::songInformation songInfo, juce::File file);
    void loadFileIntoPlayer(juce::File file);
    juce::Array<juce::File> getFileListFromDirectory(juce::String directory);

    std::unique_ptr<juce::FileChooser> chooser;
    juce::Array<juce::File> fileQueue;
    LoopState loopState;
    short queuePosition;
    float volumeSliderLevel;
    int currentUser;

    // LOOK AND FEEL

    CustomLookAndFeel* lookandFeel;

     // COLORS

    juce::Colour backgroundColor;
    juce::Colour gradientColor;

    // GEOMETRY 

    juce::Rectangle<int> backgroundGradientRect;
    juce::Rectangle<int> componentArea;

    // SUB-CLASSES
   
    juce::TextButton OPENButton{ "OPEN" };
    juce::TextButton Playlists{ "Playlists" };
    juce::TextButton Directories{ "Directories" };
    juce::TextButton Albums{ "Albums" };

    juce::ImageButton playButton{"PlayButton"};
    juce::Image playButtonImage{ juce::ImageCache::getFromMemory(BinaryData::playButtonPaused_png, BinaryData::playButtonPaused_pngSize) };
    juce::Image playButtonPlayingImage{ juce::ImageCache::getFromMemory(BinaryData::playButtonPlaying_png, BinaryData::playButtonPlaying_pngSize) };

    juce::ImageButton fastForward{ "FastForward" };
    juce::Image fastForwardImage{ juce::ImageCache::getFromMemory(BinaryData::forwardButton_png, BinaryData::forwardButton_pngSize) };
    juce::Image fastForwardImageClicked{ juce::ImageCache::getFromMemory(BinaryData::forwardButtonClicked_png, BinaryData::forwardButtonClicked_pngSize) };


    juce::ImageButton rewind{ "Rewind" };
    juce::ImageButton usersButton{ "Users" };
    juce::ImageButton volumeButton{ "Volume" };
    juce::ImageButton loopButton{ "Loop" };

    juce::Image loopButtonQueueLoopImage{ juce::ImageCache::getFromMemory(BinaryData::LoopButton_png, BinaryData::LoopButton_pngSize) };
    juce::Image loopButtonQueueLoopOnClickImage{ juce::ImageCache::getFromMemory(BinaryData::LoopButtonClicked_png, BinaryData::LoopButtonClicked_pngSize) };
    juce::Image loopButtonOneLoopImage {juce::ImageCache::getFromMemory(BinaryData::LoopButtonOne_png, BinaryData::LoopButtonOne_pngSize) };
    juce::Image loopButtonOneLoopOnClickImage = juce::ImageCache::getFromMemory(BinaryData::LoopButtonOneClicked_png, BinaryData::LoopButtonOneClicked_pngSize);

    juce::ComponentAnimator animator;

    juce::Label timePosition;
    juce::Label songTitle;
    juce::Label songLenght;
    juce::Label artistName;
    juce::Label albumName;
    juce::Label trackYear;

    juce::Slider volumeSlider;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
    playbackSlider Slider{transportSource};
    AlbumCoverDisplay albumCoverDisplay;
    MainSongComponent wsp;
    BlackoutBox blackoutBox;
    UsersSelect userSelectBox;
    SQLiteManager database{ReleaseBuild};

    juce::ListBox sideBarList;
    juce::ListBox mainList;
    Mp3DisplayBoxModel mainListModel;
    sideBarListModel sideListModel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
