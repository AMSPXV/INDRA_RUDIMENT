
#include <JuceHeader.h>
#include "songFileInfo.h"
#include <ID3v2.h>
#include <cstdint>

//==============================================================================



std::string songInfo::processYear(ID3v2::Frame::v23::TYER* pntr) {


    if (pntr != nullptr)
    {
        const uint8_t* raw_data = pntr->GetData();
        size_t data_size = pntr->GetSize();
        std::string year;

        for (size_t i = 0; i < data_size - 1; i++) {
            
            int dateInt = static_cast<int>(*raw_data);
            raw_data++;
            if (i < 1) { continue; }
            year += (char)dateInt;
        }

        return year;

    }
    else return "????";

}

songInfo::songInformation songInfo::getSongInfo(juce::File file) {

    ID3v2::Tag Mp3Tag(file.getFullPathName().toRawUTF8());
    songInfo::songInformation currentSongInfo{};

    if (Mp3Tag.IsValid())
    {
        currentSongInfo.infoAvailable = true;

        requiredFrames frames(dynamic_cast<ID3v2::Frame::v23::APIC*>(Mp3Tag.GetFrameWithName("APIC")),
            dynamic_cast<ID3v2::Frame::v23::TIT2*>(Mp3Tag.GetFrameWithName("TIT2")),
            dynamic_cast<ID3v2::Frame::v23::TYER*>(Mp3Tag.GetFrameWithName("TYER")),
            dynamic_cast<ID3v2::Frame::v23::TPE1*>(Mp3Tag.GetFrameWithName("TPE1")),
            dynamic_cast<ID3v2::Frame::v23::TALB*>(Mp3Tag.GetFrameWithName("TALB"))
        );

        // ALBUM ART

        if (frames.apic != nullptr) {

            currentSongInfo.albumImage = juce::ImageCache::getFromMemory(frames.apic->GetPictureData().data(), frames.apic->GetPictureData().size());

        }
        else {

            currentSongInfo.albumImage = juce::ImageCache::getFromMemory(BinaryData::DefaultPlay_png, BinaryData::DefaultPlay_pngSize);

        }

        frameReturnInfo titleInfo = processFrame<ID3v2::Frame::v23::TIT2>(frames.tit2);
        frameReturnInfo artistInfo = processFrame<ID3v2::Frame::v23::TPE1>(frames.tpe1);
        frameReturnInfo albumTitle = processFrame<ID3v2::Frame::v23::TALB>(frames.talb);
        std::string yearInfo = processYear(frames.tyer);

        // SONG TITLE

        if (titleInfo.frameValid != false) { currentSongInfo.SongTitle = titleInfo.frameProcessedData; }
        else { currentSongInfo.SongTitle = file.getFileName().toStdString(); }

        // ARTIST TITLE

        if (artistInfo.frameValid != false) { currentSongInfo.ArtistName = artistInfo.frameProcessedData; }
        else { currentSongInfo.ArtistName = "Uknown"; }

        // TRACK YEAR

        currentSongInfo.trackYear = yearInfo;

        // ALBUM TITLE

        if (albumTitle.frameValid != false) { currentSongInfo.AlbumName = albumTitle.frameProcessedData; }
        else { currentSongInfo.AlbumName = "Uknown"; }

    }


    return currentSongInfo;
}