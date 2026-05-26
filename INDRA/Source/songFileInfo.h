
#pragma once

#include <JuceHeader.h>
#include "AlbumCoverDisplay.h"
#include <ID3v2.h>

//==============================================================================
/*
*/


namespace songInfo 
{

    struct songInformation {

        songInformation() {
        
            infoAvailable = false;
        
        };
        bool infoAvailable;
        std::string SongTitle;
        std::string ArtistName;
        std::string AlbumName;
        std::string trackYear;
        juce::Image albumImage;

    };

    struct frameReturnInfo {

        frameReturnInfo() {};

        frameReturnInfo(bool valid, std::string value) {

            frameValid = valid;
            frameProcessedData = value;

        }

        bool frameValid;
        std::string frameProcessedData;
    };

    songInformation getSongInfo(juce::File file);

    struct requiredFrames {

        requiredFrames(ID3v2::Frame::v23::APIC* A, ID3v2::Frame::v23::TIT2* T, ID3v2::Frame::v23::TYER* Y, ID3v2::Frame::v23::TPE1* AR, ID3v2::Frame::v23::TALB* ALB) {

            apic = A;
            tit2 = T;
            tyer = Y;
            tpe1 = AR;
            talb = ALB;

        };

        ID3v2::Frame::v23::APIC* apic;
        ID3v2::Frame::v23::TIT2* tit2;
        ID3v2::Frame::v23::TYER* tyer;
        ID3v2::Frame::v23::TPE1* tpe1;
        ID3v2::Frame::v23::TALB* talb;

    };


    template <typename TP>
    frameReturnInfo processFrame(TP* pntr) {

        if (pntr != nullptr)
        {
            const uint8_t* raw = pntr->GetData();
            std::string returnString;

            for (size_t i = 0; i < pntr->GetSize() - 4; i++) {

                raw++;
                if (i < 2) { continue; }
                if ((char*)raw == "\0" || (char*)raw == "\n") { break; }
                returnString += (char*)raw;

            }

            return frameReturnInfo(true, returnString);

        }
        else return frameReturnInfo(false, "NONE");

    };

    std::string processYear(ID3v2::Frame::v23::TYER* pntr);

};
