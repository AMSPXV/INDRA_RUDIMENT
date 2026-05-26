#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "SQLiteManager.h"
#include <vector>

//==============================================================================
/*
*/



class UsersSelect  : public juce::Component,
                     public juce::ChangeListener,
                     public juce::ChangeBroadcaster
{
public:

    UsersSelect();
    ~UsersSelect() override;
    
    void setData(std::vector<User> data);
    void paint (juce::Graphics&) override;
    void resized() override;
    juce::TextButton closeButton;
    CustomLookAndFeel lookandfeel;
    
    int listSelectedUser;

    class UsersListBoxModel : public juce::ListBoxModel,
                              public juce::ChangeBroadcaster {
    public:    

        int modelSelectedUser;

        UsersListBoxModel() {

            this->modelSelectedUser = -1;

        }

        void setData(std::vector<User> data) {

            this->loadedInUsers = data;

        };

        int getNumRows() override {

            DBG(loadedInUsers.size());
            return loadedInUsers.size();

        }

        void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override {

            if (rowNumber + 1 > getNumRows()) return;

            auto hello = g.getClipBounds();

            if (rowIsSelected == true) {

                g.fillAll(juce::Colour::fromRGB(0, 66, 165));
                g.setColour(juce::Colour::fromRGB(255, 255, 255));
                g.setFont(height - 5);
                g.drawFittedText(loadedInUsers[rowNumber].name, 0, 0, width, height, juce::Justification::centred, 1);

            }
            else {

                g.fillAll(juce::Colours::black);
                g.setColour(juce::Colour::fromRGB(0, 66, 165));
                g.setFont(height - 5);
                g.drawFittedText(loadedInUsers[rowNumber].name, 0, 0, width, height, juce::Justification::centred, 1);
            }
        }
        
        void listBoxItemClicked(int row, const juce::MouseEvent&) override {

            modelSelectedUser = row;
            sendChangeMessage();

        }

        ~UsersListBoxModel() {

            removeAllChangeListeners();

        }

    private:

        std::vector<User> loadedInUsers;

    };

private:

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    UsersListBoxModel model;
    juce::ListBox list;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UsersSelect)
};
