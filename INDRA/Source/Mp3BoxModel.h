#include <JuceHeader.h>
#include "SQLiteManager.h"

#pragma once

class Mp3DisplayBoxModel : public juce::ListBoxModel,
                           public juce::ChangeBroadcaster
{

    public:

    Mp3DisplayBoxModel();
    ~Mp3DisplayBoxModel();
    int getNumRows() override;
    void setMp3Files(juce::Array<juce::File> &newMp3Files);
    juce::Component* refreshComponentForRow(int rowNumber, bool isRowSelected, juce::Component* existingComponentToUpdate) override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemDoubleClicked(int row, const juce::MouseEvent& mouse) override;
    int getSelectedMP3();

    private:

    juce::Array<juce::File> mp3Files;
    int selectedMP3;
    
};