/*
  ==============================================================================

    SideBarListModel.cpp
    Created: 16 Jun 2024 5:22:31pm
    Author:  Adam

  ==============================================================================
*/

#include "SideBarListModel.h"

int sideBarListModel::getNumRows() {

    switch (this->modelType) {

        case directory: return directory_data.size();
        case playlist: return playlist_data.size();
        case albums: return album_data.size();

    }

};

sideBarListModel::sideBarListModel() {

    this->modelType = directory;

};

sideBarListModel::~sideBarListModel() {


};

void sideBarListModel::setDirectoryData(juce::Array<std::string> data) {

    this->directory_data = data;
    album_data.clear();
    playlist_data.clear();

}

void sideBarListModel::setAlbumData(juce::Array<sideBarDataTypes::albumData> data) {

    this->album_data = data;
    directory_data.clear();
    playlist_data.clear();

}

void sideBarListModel::setPlaylistData(juce::Array<sideBarDataTypes::playlistData> data) {

    this->playlist_data = data;
    directory_data.clear();
    album_data.clear();

}

void sideBarListModel::changeModelType(sideBarItemType newModelType) {

    this->modelType = newModelType;

}; 

void sideBarListModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) {

    if (rowNumber + 1 > getNumRows()) return;

    auto clipBounds = g.getClipBounds();
    float cornerRadius = height / 2 * 0.3;
    juce::Path path;

    if (rowIsSelected) {

        
        path.addRoundedRectangle(clipBounds.toFloat(), cornerRadius);
        g.reduceClipRegion(path);
        g.fillAll(juce::Colours::black);
        path.clear();
        path.addRoundedRectangle(clipBounds.toFloat().reduced(1.2), cornerRadius);
        g.reduceClipRegion(path);
        g.fillAll(juce::Colours::white);
        g.reduceClipRegion(clipBounds);

        auto clipBottomRight = clipBounds.getBottomRight();
        auto clipTopLeft = clipBounds.getTopLeft();


        switch (this->modelType) {

            case directory:
            {
                g.drawImageWithin(
                    juce::ImageCache::getFromMemory(BinaryData::directoryIconOnHover_png, BinaryData::directoryIconOnHover_pngSize),
                    clipTopLeft.x+3, clipTopLeft.y + 3, clipBottomRight.y * 0.8, clipBottomRight.y * 0.8,
                    juce::RectanglePlacement::centred
                );

               g.setColour(juce::Colours::black);
               g.drawFittedText(directory_data[rowNumber], (clipBottomRight.x / 8), clipBottomRight.y / 3,
                                clipBottomRight.x / 2, clipBottomRight.y / 2,
                                juce::Justification::left, 1
               );

            }
            break;

            case playlist:
            {
                g.setColour(juce::Colours::black);
                g.drawFittedText(playlist_data[rowNumber].playlist_name, (clipBottomRight.x / 8), clipBottomRight.y / 3,
                    clipBottomRight.x / 2, clipBottomRight.y / 2,
                    juce::Justification::left, 1
                );

            }
            break;

            case albums:
            {
                juce::Image* desaturate = new juce::Image(album_data[rowNumber].album_art.createCopy());
                desaturate->desaturate();
                g.drawImageWithin(
                    *desaturate,
                    clipTopLeft.x + 3, clipTopLeft.y + 3, clipBottomRight.y * 0.8, clipBottomRight.y * 0.8,
                    juce::RectanglePlacement::centred
                );
                delete desaturate;
                g.setColour(juce::Colours::black);
                g.drawFittedText(album_data[rowNumber].album_name, (clipBottomRight.x / 8), clipBottomRight.y / 3,
                    clipBottomRight.x / 2, clipBottomRight.y / 2,
                    juce::Justification::left, 1
                );
            }
            break;
        }


    }
    else {

        path.addRoundedRectangle(clipBounds.toFloat(), cornerRadius);
        g.reduceClipRegion(path);
        g.fillAll(juce::Colours::white);
        path.clear();
        path.addRoundedRectangle(clipBounds.toFloat().reduced(1.2), cornerRadius);
        g.reduceClipRegion(path);
        g.fillAll(juce::Colours::black);
        g.reduceClipRegion(clipBounds);

        auto clipBottomRight = clipBounds.getBottomRight();
        auto clipTopLeft = clipBounds.getTopLeft();

        switch (this->modelType) {

        case directory:
        {
            g.drawImageWithin(
                juce::ImageCache::getFromMemory(BinaryData::directoryIcon_png, BinaryData::directoryIcon_pngSize),
                clipTopLeft.x + 3, clipTopLeft.y + 3, clipBottomRight.y * 0.8, clipBottomRight.y * 0.8,
                juce::RectanglePlacement::centred
            );

            g.setColour(juce::Colours::white);
            g.drawFittedText(directory_data[rowNumber], (clipBottomRight.x / 8), clipBottomRight.y / 3,
                clipBottomRight.x / 2, clipBottomRight.y / 2,
                juce::Justification::left, 1
            );
        }
        break;

        case playlist:
        {
            g.setColour(juce::Colours::white);
            g.drawFittedText(playlist_data[rowNumber].playlist_name, (clipBottomRight.x / 8), clipBottomRight.y / 3,
                clipBottomRight.x / 2, clipBottomRight.y / 2,
                juce::Justification::left, 1
            );

        }
        break;

        case albums:
        {
            g.drawImageWithin(
                album_data[rowNumber].album_art,
                clipTopLeft.x + 3, clipTopLeft.y + 3, clipBottomRight.y * 0.8, clipBottomRight.y * 0.8,
                juce::RectanglePlacement::centred
            );

            g.setColour(juce::Colours::white);
            g.drawFittedText(album_data[rowNumber].album_name, (clipBottomRight.x / 8), clipBottomRight.y / 3,
                clipBottomRight.x / 2, clipBottomRight.y / 2,
                juce::Justification::left, 1
            );

        }
        break;
        }

    }

};

void sideBarListModel::listBoxItemDoubleClicked(int row, const juce::MouseEvent& mouse) {

    selectedElementIndex = row;
    sendChangeMessage();

};

sideBarListModel::sideBarItemType sideBarListModel::getModelType() {
    return this->modelType;
}

sideBarDataTypes::albumData sideBarListModel::getAlbumData(int index) {

    return this->album_data[index];

};

std::string sideBarListModel::getDirectory(int index) {

    return this->directory_data[index];

};