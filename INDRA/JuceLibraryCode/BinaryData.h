/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   directoryIcon_png;
    const int            directoryIcon_pngSize = 61330;

    extern const char*   directoryIconOnHover_png;
    const int            directoryIconOnHover_pngSize = 46184;

    extern const char*   DefaultPlay_png;
    const int            DefaultPlay_pngSize = 119775;

    extern const char*   LoopButtonClicked_png;
    const int            LoopButtonClicked_pngSize = 58898;

    extern const char*   LoopButtonOne_png;
    const int            LoopButtonOne_pngSize = 57478;

    extern const char*   LoopButtonOneClicked_png;
    const int            LoopButtonOneClicked_pngSize = 61853;

    extern const char*   LoopButton_png;
    const int            LoopButton_pngSize = 55252;

    extern const char*   VolumeButtonOff_png;
    const int            VolumeButtonOff_pngSize = 25296;

    extern const char*   VolumeButtonOffClicked_png;
    const int            VolumeButtonOffClicked_pngSize = 27022;

    extern const char*   playButtonPlaying_png;
    const int            playButtonPlaying_pngSize = 85456;

    extern const char*   backwardsButton_png;
    const int            backwardsButton_pngSize = 14033;

    extern const char*   backwardsButtonClicked_png;
    const int            backwardsButtonClicked_pngSize = 14385;

    extern const char*   forwardButtonClicked_png;
    const int            forwardButtonClicked_pngSize = 14354;

    extern const char*   MohaveRegular_ttf;
    const int            MohaveRegular_ttfSize = 40760;

    extern const char*   forwardButton_png;
    const int            forwardButton_pngSize = 13953;

    extern const char*   VolumeButton_png;
    const int            VolumeButton_pngSize = 39246;

    extern const char*   UserLogoPressed_png;
    const int            UserLogoPressed_pngSize = 43463;

    extern const char*   UserLogo_png;
    const int            UserLogo_pngSize = 60861;

    extern const char*   ALBUMPLACEHOLDER_png;
    const int            ALBUMPLACEHOLDER_pngSize = 4426;

    extern const char*   VolumeButtonClicked_png;
    const int            VolumeButtonClicked_pngSize = 41618;

    extern const char*   playButtonPaused_png;
    const int            playButtonPaused_pngSize = 307177;

    extern const char*   INDRALogo_png;
    const int            INDRALogo_pngSize = 67653;

    extern const char*   INDRALogo_svg;
    const int            INDRALogo_svgSize = 66250;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 23;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
