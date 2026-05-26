#ifdef NDEBUG

#define FILEPATH true

#else

#define FILEPATH false

#endif


#include <JuceHeader.h>
#include <fstream>
#include "MainComponent.h"
#include "AlbumCoverDisplay.h"
#include "SQLiteManager.h"
#include "songFileInfo.h"
#include "BlackoutBox.h"
#include "UsersSelect.h"
#include "Mp3BoxModel.h"

// TEMP INCLUDES

#include "MainSongComponent.h"

//==============================================================================
MainComponent::MainComponent(int user)
    : state (Stopped) 
    , loopState (NoLoop)
    , volumeSliderLevel(100)
    , backgroundColor(juce::Colour(23, 23, 23))
    , gradientColor(juce::Colour(56, 56, 139))
    , queuePosition(0)
{
    currentUser = user;
    setSize(800, 450);
    componentArea = getLocalBounds();
    backgroundGradientRect = componentArea.removeFromBottom(componentArea.getHeight() / 5);

    //  FORMAT MANAGER

    formatManager.registerBasicFormats();

    // TRANSPORT SOURCE

    transportSource.addChangeListener(this);

    // AUDIO OUTPUT SETUP

    setAudioChannels(0, 2);

    // LOOK AND FEEL

    lookandFeel = new CustomLookAndFeel();
    juce::LookAndFeel::setDefaultLookAndFeel(lookandFeel);

    // LOAD FILE BUTTON


    OPENButton.onClick = [&]() {

        OPENbuttonClicked();

    };
    
    addAndMakeVisible(OPENButton);

    // PLAY BUTTON
    
    playButton.setImages(false, true, true,
    
        playButtonImage,1.0f,juce::Colours::transparentWhite,
        playButtonImage, 1.0f, juce::Colour::fromRGBA(255,255,255, 125),
        playButtonPlayingImage, 1.0f, juce::Colour::fromRGBA(255, 255, 255, 125)
    );
    playButton.setClickingTogglesState(true);
    playButton.setEnabled(false);
    playButton.onClick = [this]() { playButtonClicked(); };

    addAndMakeVisible(playButton);

    // SLIDER

    addAndMakeVisible(Slider);
    Slider.slider.onDragStart = [&]() {

        stopTimer();

        };

    Slider.slider.onDragEnd = [&]() {

        transportSource.setPosition(Slider.slider.getValue());
        if (state != Stopped) startTimer(100);

        };

    // FAST FORWARD

    fastForward.setImages(false, true, true,

        fastForwardImage, 1.0f, juce::Colours::transparentWhite,
        fastForwardImage, 1.0f, juce::Colours::transparentWhite,
        fastForwardImageClicked, 1.0f, juce::Colours::transparentWhite
    );
    fastForward.onClick = [&]() {

        transportSource.setPosition(transportSource.getLengthInSeconds());

        };

    addAndMakeVisible(fastForward);

    // REWIND

    rewind.setImages(false, true, true,

        juce::ImageCache::getFromMemory(BinaryData::backwardsButton_png, BinaryData::backwardsButton_pngSize), 1.0f, juce::Colours::transparentWhite,
        juce::ImageCache::getFromMemory(BinaryData::backwardsButton_png, BinaryData::backwardsButton_pngSize), 1.0f, juce::Colours::transparentWhite,
        juce::ImageCache::getFromMemory(BinaryData::backwardsButtonClicked_png, BinaryData::backwardsButtonClicked_pngSize), 1.0f, juce::Colours::transparentWhite
    );
    rewind.onClick = [&]() {

        if ((queuePosition - 1) >= 0 && transportSource.getCurrentPosition() <= 5) {

            queuePosition--;
            loadFileIntoPlayer(fileQueue[queuePosition]);
            playButton.getToggleState() ? changeState(Starting) : (void)0;


        } else transportSource.setPosition(0.0);
     };
    addAndMakeVisible(rewind);

    // USERS BUTTON

    usersButton.setImages(false, true, true,

        juce::ImageCache::getFromMemory(BinaryData::UserLogo_png, BinaryData::UserLogo_pngSize), 1.0f, juce::Colours::transparentWhite,
        juce::Image(), 0.8f, juce::Colours::transparentWhite,
        juce::ImageCache::getFromMemory(BinaryData::UserLogoPressed_png, BinaryData::UserLogoPressed_pngSize), 1.0f, juce::Colours::transparentWhite

    );

    usersButton.onClick = [&]() {
        usersButtonClicked();
    };

    addAndMakeVisible(usersButton);

    // ALBUM COVER DISPLAY

    addAndMakeVisible(albumCoverDisplay);

    // VOLUME BUTTON

    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    volumeSlider.setRange(0, 120);
    volumeSlider.setAlwaysOnTop(true);
    volumeSlider.setValue(volumeSliderLevel);

    volumeSlider.onValueChange = [&]() {

        volumeSliderLevel = volumeSlider.getValue();

        };

    addChildComponent(volumeSlider);

    juce::Image volumeButtonImage = juce::ImageCache::getFromMemory(BinaryData::VolumeButton_png, BinaryData::VolumeButton_pngSize);
    juce::Image volumeButtonImageOnClick = juce::ImageCache::getFromMemory(BinaryData::VolumeButtonClicked_png, BinaryData::VolumeButtonClicked_pngSize);

    volumeButton.setImages(false, true, true,
        volumeButtonImage, 1.0f, juce::Colours::transparentWhite,
        volumeButtonImage, 1.0f, juce::Colours::transparentWhite,
        volumeButtonImageOnClick, 1.0f, juce::Colours::transparentWhite);

    volumeButton.onClick = [&]() {

        if (!volumeSlider.isVisible()) {

            volumeSlider.setVisible(true);

        }
        else volumeSlider.setVisible(false);

    };

    addAndMakeVisible(volumeButton);

    // LOOP BUTTON


    loopButton.setImages(false, true, true,
        loopButtonQueueLoopImage, 0.2f, juce::Colours::transparentWhite,
        loopButtonQueueLoopImage, 0.2f, juce::Colours::transparentWhite,
        loopButtonQueueLoopOnClickImage, 1.0f, juce::Colours::transparentWhite);

    loopButton.onClick = [&]() {

        loopButtonClicked();

    };

    addAndMakeVisible(loopButton);

    // TIME POSITION

    timePosition.setText("-:--", juce::dontSendNotification);
    timePosition.setColour(juce::Label::textColourId,juce::Colours::white);
    timePosition.setJustificationType(juce::Justification::centredLeft);
    timePosition.setMinimumHorizontalScale(1.0f);
    addAndMakeVisible(timePosition);

    // BLACKOUT BOX

    addAndMakeVisible(blackoutBox);
    blackoutBox.setAlpha(0);
    blackoutBox.setInterceptsMouseClicks(false, false);
    blackoutBox.toFront(false);

    // USERS SELECT

    userSelectBox.setAlwaysOnTop(true);
    userSelectBox.setData(database.getUsers());
    userSelectBox.addChangeListener(this);
    userSelectBox.closeButton.onClick = [this]() {usersButtonClicked(); };
    addChildComponent(userSelectBox);

    // PLAYLISTS, DIRECTORIES, SONGS, ALBUMS

    addAndMakeVisible(Playlists);
    Playlists.onClick = [this]() {tabButtonClicked(1); };
    addAndMakeVisible(Directories);
    Directories.onClick = [this]() {tabButtonClicked(0); };
    addAndMakeVisible(Albums);
    Albums.onClick = [this]() {tabButtonClicked(2); };
    tabButtonClicked(0);

    // SIDEBAR LIST

    sideBarList.setModel(&sideListModel);
    addAndMakeVisible(sideBarList);
    sideBarList.setMouseMoveSelectsRows(true);
    sideListModel.addChangeListener(this);

    // MAIN LIST

    mainList.setModel(&mainListModel);
    mainListModel.addChangeListener(this);
    addAndMakeVisible(mainList);

    // SONG INFORMATION LABELS

    songTitle.setColour(juce::Label::textColourId, juce::Colours::white);
    songTitle.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(songTitle);

    songLenght.setColour(juce::Label::textColourId, juce::Colour::fromRGBA(0, 0, 0, 200));
    songLenght.setText(" ? : ? ? ",juce::dontSendNotification);
    songLenght.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(songLenght);

    artistName.setColour(juce::Label::textColourId, juce::Colour::fromRGB(0, 66, 165));
    artistName.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(artistName);

    albumName.setColour(juce::Label::textColourId, juce::Colour::fromRGBA(0, 0, 0, 200));
    albumName.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(albumName);

    trackYear.setColour(juce::Label::textColourId, juce::Colour::fromRGBA(255, 255, 255, 200));
    trackYear.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(trackYear);
    
}

MainComponent::~MainComponent()
{
    shutdownAudio();
    DBG(currentUser);
    std::string filepath;
    if (!FILEPATH) {

        filepath = "../../../Storage/lastUser.txt";

    }
    else {

        filepath = "../../../../../../Storage/lastUser.txt";
    }

    std::ofstream userFile(filepath);
    userFile << currentUser;
    userFile.close();
    delete lookandFeel;

}

void MainComponent::paint (juce::Graphics& g)
{

    // FILL

    g.fillAll(backgroundColor);
   
    // GRADIENT

    auto bottomLeft = backgroundGradientRect.getBottomLeft();
    auto topLeft = backgroundGradientRect.getTopLeft();
    g.setGradientFill(juce::ColourGradient(backgroundColor,topLeft.x, topLeft.y, gradientColor, bottomLeft.x, bottomLeft.y, false));
    g.fillRect(backgroundGradientRect);


}

void MainComponent::resized()
{
    componentArea = getLocalBounds();
    auto footerArea = componentArea.removeFromBottom(componentArea.getHeight() / 4);
    auto footerAreaWidth = footerArea.getWidth();
    auto footerAreaHeight = footerArea.getHeight();

    auto headerArea = componentArea.removeFromTop(componentArea.getHeight() / 24);

    auto sidebarArea = componentArea.removeFromLeft(componentArea.getWidth() / 3);

    auto defaultUtilityButtonSize = juce::jmax(footerAreaHeight / 3, 20);

    // BACKGROUND GRADIENT POS
    
    backgroundGradientRect = getLocalBounds().removeFromBottom(getLocalBounds().getHeight() / 5);
    
    // OPEN BUTTON POS

    auto loadFileButtonPosition = headerArea.getTopLeft();
    loadFileButtonPosition.addXY(defaultUtilityButtonSize, 5);
    auto loadFileButtonWidth = juce::jmax(80, headerArea.getWidth() / 15);
    auto loadFileButtonHeight = juce::jmax(20, headerArea.getHeight());

    OPENButton.setBounds(loadFileButtonPosition.x,loadFileButtonPosition.y,loadFileButtonWidth,loadFileButtonHeight);

    // PLAY BUTTON POS

    auto playButtonSize = juce::jmax<int>(50, footerArea.getHeight());
    auto playButtonPosition = footerArea.getTopLeft();
    playButtonPosition.addXY(defaultUtilityButtonSize, 0);
    playButtonPosition.addXY(footerAreaWidth / 5,0);
    auto resizedPlayButtonImage = gin::applyResize(playButtonImage, playButtonSize, playButtonSize);
    auto resizedPlayButtonPlayingImage = gin::applyResize(playButtonPlayingImage, playButtonSize, playButtonSize);

    playButton.setImages(false, true, true,

        resizedPlayButtonImage, 1.0f, juce::Colours::transparentWhite,
        resizedPlayButtonImage, 1.0f, juce::Colour::fromRGBA(255, 255, 255, 125),
        resizedPlayButtonPlayingImage, 1.0f, juce::Colour::fromRGBA(255, 255, 255, 125)
    );

    playButton.setBounds(playButtonPosition.x,playButtonPosition.y,playButtonSize,playButtonSize);

    // SLIDER POS

    auto sliderPosition = juce::Point<int>(playButtonPosition.x + playButtonSize + defaultUtilityButtonSize*3, footerArea.getCentreY());
    Slider.setBounds(sliderPosition.x,sliderPosition.y,footerAreaWidth - playButtonSize - defaultUtilityButtonSize * 4 - footerAreaWidth / 5 - 10, footerAreaHeight / 10);

    // FAST FORWARD POS 

    fastForward.setBounds(playButtonPosition.x + playButtonSize,sliderPosition.y - (defaultUtilityButtonSize/4) - 8, defaultUtilityButtonSize, defaultUtilityButtonSize);

    // REWIND POS

    rewind.setBounds(playButtonPosition.x - defaultUtilityButtonSize, sliderPosition.y - (defaultUtilityButtonSize / 4) - 8, defaultUtilityButtonSize, defaultUtilityButtonSize);

    // USERS POS

    usersButton.setBounds(loadFileButtonPosition.x - defaultUtilityButtonSize,loadFileButtonPosition.y,defaultUtilityButtonSize,defaultUtilityButtonSize);

    // ALBUM COVER POS

    auto albumCoverDisplayPos = footerArea.getTopLeft();
    albumCoverDisplayPos.addXY(5, -25);
    auto albumCoverSize = footerAreaWidth / 5 - 10;

    albumCoverDisplay.setBounds(albumCoverDisplayPos.x, playButton.getBottom() - albumCoverSize - 15, albumCoverSize, albumCoverSize);

    // VOLUME BUTTON POS

    volumeButton.setBounds(playButtonPosition.x + playButtonSize + defaultUtilityButtonSize, sliderPosition.y - (defaultUtilityButtonSize / 4) - 8, defaultUtilityButtonSize, defaultUtilityButtonSize);

    volumeSlider.setBounds(volumeButton.getX() + volumeButton.getWidth()/2, volumeButton.getY() - defaultUtilityButtonSize * 2, defaultUtilityButtonSize / 5, defaultUtilityButtonSize * 2);

    // LOOP BUTTON POS

    loopButton.setBounds(playButtonPosition.x + playButtonSize + defaultUtilityButtonSize*2, sliderPosition.y - (defaultUtilityButtonSize / 4) - 8, defaultUtilityButtonSize, defaultUtilityButtonSize);
    
    // TIME POSITION

    
    timePosition.setBounds(sliderPosition.x,sliderPosition.y - Slider.getHeight()*3, Slider.getWidth() / 6, defaultUtilityButtonSize / 2);
    timePosition.setFont(juce::Font(timePosition.getHeight()));
    
    // BLACKOUT BOX

    blackoutBox.setBounds(getLocalBounds());

    // USERS BOX

    auto usersSelectBoxWidth = getWidth() / 4;
    auto usersSelectBoxHeight = getHeight() / 5 * 4;
    auto usersSelectPosition = getLocalBounds().getCentre();

    userSelectBox.setBounds(usersSelectPosition.x, usersSelectPosition.y, usersSelectBoxWidth, usersSelectBoxHeight);
    userSelectBox.setCentreRelative(0.5, 0.5);

    // PLAYLISTS, DIRECTORIES, SONGS, ALBUMS POS.

    auto sidebarTopLeft = sidebarArea.getTopLeft();
    sidebarTopLeft.addXY(10, 0);
    auto sideBarWidth = sidebarArea.getWidth();
    auto sideBarHeight = sidebarArea.getHeight();

    Directories.setBounds(sidebarTopLeft.x, sidebarTopLeft.y + sideBarHeight / 6, sideBarWidth / 3, sideBarHeight / 20);
    Playlists.setBounds(sidebarTopLeft.x + (sideBarWidth / 3), sidebarTopLeft.y + sideBarHeight / 6, sideBarWidth / 3, sideBarHeight / 20);
    Albums.setBounds(sidebarTopLeft.x + (sideBarWidth / 3) * 2, sidebarTopLeft.y + sideBarHeight / 6, sideBarWidth / 3, sideBarHeight / 20);

    // SIDEBAR LIST

    sideBarList.setBounds(sidebarTopLeft.x, sidebarTopLeft.y + sideBarHeight /6 + sideBarHeight / 20 + 5, sideBarWidth - 2, sideBarHeight - albumCoverSize);

    // MAIN LIST

    mainList.setBounds(sidebarTopLeft.x + (sideBarWidth / 4) * 4 + 10, sidebarTopLeft.y + sideBarHeight / 6,componentArea.getWidth() - 30, sideBarHeight - footerAreaHeight);
    mainList.setRowHeight((sidebarTopLeft.y + sideBarHeight / 6, componentArea.getWidth() - 30) / 8);

    // SONG INFO LABELS

    songLenght.setBounds(sliderPosition.x + timePosition.getWidth(), sliderPosition.y - Slider.getHeight() * 3, Slider.getWidth() / 6, defaultUtilityButtonSize / 2);
    songLenght.setFont(juce::Font(timePosition.getHeight()));

    songTitle.setBounds(sliderPosition.x, sliderPosition.y - Slider.getHeight() * 3 - defaultUtilityButtonSize, Slider.getWidth() / 2, defaultUtilityButtonSize);
    songTitle.setFont(juce::Font(defaultUtilityButtonSize));

    artistName.setBounds(sliderPosition.x + songTitle.getWidth(), sliderPosition.y - Slider.getHeight() * 3 - defaultUtilityButtonSize, Slider.getWidth() / 2, defaultUtilityButtonSize);
    artistName.setFont(juce::Font(defaultUtilityButtonSize));

    albumName.setBounds(artistName.getX(), sliderPosition.y - Slider.getHeight() * 3, Slider.getWidth() / 2 / 4 * 3, defaultUtilityButtonSize / 2);
    albumName.setFont(juce::Font(timePosition.getHeight()));

    trackYear.setBounds(artistName.getX() + albumName.getWidth(), sliderPosition.y - Slider.getHeight() * 3, Slider.getWidth() / 2 / 4, defaultUtilityButtonSize / 2);
    trackYear.setFont(juce::Font(timePosition.getHeight()));
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {

    if (source == &transportSource)
    {
        if (transportSource.isPlaying()) {
            changeState(Playing);
        }
        else if ((state == Stopping) || (state == Playing)) {

            if (loopState == SongLooped) {

                loadFileIntoPlayer(fileQueue[queuePosition]);
                changeState(Starting);

            }
            else if (fileQueue[queuePosition] != fileQueue.getLast()) {

                queuePosition++;
                loadFileIntoPlayer(fileQueue[queuePosition]);
                changeState(Starting);

            }
            else if (loopState == QueueLooped) {

                queuePosition = 0;
                loadFileIntoPlayer(fileQueue[queuePosition]);
                changeState(Starting);
            }
            else { changeState(Stopped); }
        }
        else if (state == Pausing)
            changeState(Paused);
    }
    else if (source == &userSelectBox) {

        currentUser = database.getUserIDByOrder(userSelectBox.listSelectedUser);
        usersButtonClicked();
        switch (sideListModel.getModelType()) {

        case sideBarListModel::directory: tabButtonClicked(0); break;
        case sideBarListModel::playlist: tabButtonClicked(1); break;
        case sideBarListModel::albums: tabButtonClicked(2); break;

        }
    }
    else if (source == &mainListModel) {

        queuePosition = mainListModel.getSelectedMP3();
        loadFileIntoPlayer(fileQueue[queuePosition]);
        if ((state == Stopping) || (state == Playing)) {

            changeState(Starting);

        }

    }
    else if (source == &sideListModel) {

        switch (sideListModel.getModelType()) {

            case sideBarListModel::directory: {

                if (state == Playing) {

                    changeState(Stopping);

                }
                std::string directory = sideListModel.getDirectory(sideListModel.selectedElementIndex);
                juce::Array<juce::File> tempQueue = getFileListFromDirectory(directory);
                if (tempQueue.isEmpty()) break;
                fileQueue = tempQueue;
                tempQueue.clear();
                loadFileIntoPlayer(fileQueue[0]);
                mainListModel.setMp3Files(fileQueue);
                mainList.updateContent();
                lookAndFeelChanged();

            }break;

            case sideBarListModel::albums: {

                if (state == Playing) {

                    changeState(Stopping);

                }
                sideBarDataTypes::albumData albumdata = sideListModel.getAlbumData(sideListModel.selectedElementIndex);
                fileQueue = database.getAlbum(albumdata.album_id);
                loadFileIntoPlayer(fileQueue[0]);
                mainListModel.setMp3Files(fileQueue);
                mainList.updateContent();
                lookAndFeelChanged();

            }break;

        };

    }

};

void MainComponent::changeState(TransportState newState)
{
    if (state != newState)
    {
        state = newState;

        switch (state)
        {
        case Stopped:                           
            playButton.setEnabled(true);
            if (playButton.getToggleState() == true) playButton.setToggleState(false, false);
            transportSource.setPosition(0.0);
            break;

        case Starting:
            transportSource.start();
            startTimer(100);
            break;

        case Playing:                           
            break;

        case Stopping:                     
            transportSource.stop();
            stopTimer();
            break;

        case Pausing:
            transportSource.stop();
            break;

        }
    }
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{

    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.setGain(volumeSliderLevel/100);
    transportSource.getNextAudioBlock(bufferToFill);

}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void MainComponent::releaseResources()
{
    transportSource.releaseResources();
}


void MainComponent::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Izberite datoteko...",
        juce::File{},
        "*.mp3");
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)    
        {
            auto file = fc.getResult();
            if (file != juce::File{}) {

                if (fileQueue.isEmpty()) {

                    fileQueue.add(file);
                    queuePosition = 0;
                    loadFileIntoPlayer(fileQueue.getLast());

                }
                else {

                    fileQueue.add(file);

                }

            }
            

        });
}

void MainComponent::DirectoryButtonClicked() {

    chooser = std::make_unique<juce::FileChooser>("Izberite mapo...",
        juce::File{});
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectDirectories;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File{}) {

                if (state == Playing) {

                    changeState(Stopping);

                }
                database.addDirectory(file.getFullPathName().toRawUTF8(),currentUser);
                
                if (sideListModel.getModelType() == sideBarListModel::directory) tabButtonClicked(0);
                fileQueue = getFileListFromDirectory(file.getFullPathName().toRawUTF8());
                loadFileIntoPlayer(fileQueue[0]);
                mainListModel.setMp3Files(fileQueue);
                mainList.updateContent();
                if (fileQueue.isEmpty()) mainList.repaint();
            }


        });

}

void MainComponent::loadFileIntoPlayer(juce::File file)
{
        auto* reader = formatManager.createReaderFor(file);

        if (reader != nullptr)
        {
            songInfo::songInformation retrievedInfo = songInfo::getSongInfo(file);
            auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            database.addSong(retrievedInfo, file.getFullPathName().toRawUTF8(), 1, transportSource.getLengthInSeconds());
            Slider.changeSliderRange(transportSource.getLengthInSeconds());
            updateSongInfoLabels(retrievedInfo, file);
            playButton.setEnabled(true);
            readerSource.reset(newSource.release());
        }

}



void MainComponent::playButtonClicked()
{
    auto toggleState = playButton.getToggleState();

    if (toggleState != true) {

        changeState(Pausing);

    }
    else

    {
        changeState(Starting);
    }
  
}


void MainComponent::timerCallback()
{
    double timePositionValue = transportSource.getCurrentPosition();
    Slider.updateSlider(timePositionValue);
    int timePositionSeconds = (int)(timePositionValue) % 60;
    std::string timePositionString;
    timePositionSeconds < 10 ? 
            timePositionString = std::to_string((int)timePositionValue / 60) + ':' + '0' + std::to_string(timePositionSeconds) :
            timePositionString = std::to_string((int)timePositionValue / 60) + ':' + std::to_string(timePositionSeconds);
    timePosition.setText(timePositionString, juce::dontSendNotification);

}

void MainComponent::OPENbuttonClicked()
{
    juce::PopupMenu m;
    m.addItem(1, "Song");
    m.addItem(2, "Directory");

    m.showMenuAsync(juce::PopupMenu::Options(),
        [this](int result)
        {
            if (result == 0)
            {

            }
            else if (result == 1)
            {

                openButtonClicked();

            }
            else if (result == 2)
            {

                DirectoryButtonClicked();

            }
        });

}
void MainComponent::loopButtonClicked() {

    switch (loopState) {

    case NoLoop:
    {
        loopState = QueueLooped;
        loopButton.setImages(false, true, true,
            loopButtonQueueLoopImage, 1.0f, juce::Colours::transparentWhite,
            loopButtonQueueLoopImage, 1.0f, juce::Colours::transparentWhite,
            loopButtonQueueLoopOnClickImage, 1.0f, juce::Colours::transparentWhite);
        break;
    }

    case SongLooped:
    {
        loopState = NoLoop;
        loopButton.setImages(false, true, true,
            loopButtonQueueLoopImage, 0.2f, juce::Colours::transparentWhite,
            loopButtonQueueLoopImage, 0.2f, juce::Colours::transparentWhite,
            loopButtonQueueLoopOnClickImage, 1.0f, juce::Colours::transparentWhite);
        break;
    }

    case QueueLooped:
    {
        loopState = SongLooped;
        loopButton.setImages(false, true, true,
            loopButtonOneLoopImage, 1.0f, juce::Colours::transparentWhite,
            loopButtonOneLoopImage, 1.0f, juce::Colours::transparentWhite,
            loopButtonOneLoopOnClickImage, 1.0f, juce::Colours::transparentWhite);
        break;
    }

    }

}

void MainComponent::blackoutBoxToggle()
{
    if (blackoutBox.getAlpha() == 1.0) {

        animator.fadeOut(&blackoutBox, 500);
        blackoutBox.setInterceptsMouseClicks(false, false);
        blackoutBox.toFront(false);
    }
    else
    {
        animator.fadeIn(&blackoutBox, 500);
        blackoutBox.toFront(true);
        blackoutBox.setInterceptsMouseClicks(true, false);
    }

}

void MainComponent::usersButtonClicked()
{

    blackoutBoxToggle();
    if (userSelectBox.isVisible()) userSelectBox.setVisible(false); else
        userSelectBox.setVisible(true);
    userSelectBox.setData(database.getUsers());

}

void MainComponent::updateSongInfoLabels(songInfo::songInformation songInfo,juce::File file) {

    if (songInfo.infoAvailable == false) {

        albumCoverDisplay.loadNewImage(juce::ImageCache::getFromMemory(BinaryData::DefaultPlay_png, BinaryData::DefaultPlay_pngSize));
        artistName.setText("Unknown", juce::dontSendNotification);
        albumName.setText("Uknown",juce::dontSendNotification);
        songTitle.setText(file.getFileNameWithoutExtension(), juce::dontSendNotification);
        trackYear.setText("(????)",juce::dontSendNotification);
    }
    else {

        albumCoverDisplay.loadNewImage(songInfo.albumImage);
        artistName.setText(songInfo.ArtistName, juce::dontSendNotification);
        albumName.setText(songInfo.AlbumName, juce::dontSendNotification);
        songTitle.setText(songInfo.SongTitle, juce::dontSendNotification);
        trackYear.setText("(" + songInfo.trackYear + ")", juce::dontSendNotification);

    }

    int songLenghtInSeconds = transportSource.getLengthInSeconds();
    std::string songLenghtString = (songLenghtInSeconds % 60 < 10) ? std::to_string(songLenghtInSeconds / 60) + ":0" + std::to_string(songLenghtInSeconds % 60)
                                                                   : std::to_string(songLenghtInSeconds / 60) + ":" + std::to_string(songLenghtInSeconds % 60);
    songLenght.setText(songLenghtString, juce::dontSendNotification);

};

juce::Array<juce::File> MainComponent::getFileListFromDirectory(juce::String directory) {

    juce::File directoryFile(directory.toRawUTF8());
    juce::Array<juce::File> returnArray = directoryFile.findChildFiles(2, false, "*.mp3");
    return returnArray;
}

void MainComponent::tabButtonClicked(int button) {

    // 0 - directories
    // 1 - playlists
    // 2 - albums

    switch (button) {

        case 0: {
    
            auto data = database.getDirectories(currentUser);
            sideListModel.changeModelType(sideBarListModel::directory);
            sideListModel.setDirectoryData(data);
            sideBarList.updateContent();
            lookAndFeelChanged();
    
        } break;

        case 1: {
        
            auto data = database.getPlaylistForUser(currentUser);
            sideListModel.changeModelType(sideBarListModel::playlist);
            sideListModel.setPlaylistData(data);
            sideBarList.updateContent();
            lookAndFeelChanged();
        
        } break;

        case 2: {
        
            auto data = database.getAlbums();
            sideListModel.changeModelType(sideBarListModel::albums);
            sideListModel.setAlbumData(data);
            sideBarList.updateContent();
            lookAndFeelChanged();
           
        
        } break;

    }


}