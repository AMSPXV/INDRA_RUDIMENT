#include <JuceHeader.h>
#include "UsersSelect.h"
#include "SQLiteManager.h"

//==============================================================================
UsersSelect::UsersSelect()
{
    addAndMakeVisible(closeButton);
    closeButton.setButtonText("CLOSE");
    model.addChangeListener(this);
    list.setModel(&model);
    addAndMakeVisible(list);
    list.setMouseMoveSelectsRows(true);
}

void UsersSelect::setData(std::vector<User> data) {

    this->model.setData(data);

}

UsersSelect::~UsersSelect()
{
}

void UsersSelect::paint (juce::Graphics& g)
{
    auto localBounds = getLocalBounds(); 

    auto localbounds = getLocalBounds();
    auto center = localbounds.getCentre().toFloat();
    auto topleft = localbounds.getTopLeft().toFloat();
    auto height = getHeight();
    auto width = getWidth();
    float cornerRadius = height / 2 * 0.1;

    juce::Path path;
    path.addRoundedRectangle(localbounds.toFloat(), cornerRadius);
    g.reduceClipRegion(path);
    g.setColour(juce::Colours::black);
    g.fillAll();
    path.clear();


    g.setColour(juce::Colour::fromRGB(0, 66, 165));
    g.setFont(height / 15   );
    g.drawFittedText("USERS", getLocalBounds().removeFromTop(height / 10), juce::Justification::centred, 1);

}

void UsersSelect::resized()
{
    auto localBoundz = getLocalBounds();
    auto center = localBoundz.getCentre();
    auto height = getHeight();
    auto width = getWidth();

    auto buttonWidth = width / 5;
    auto buttonHeight = juce::jmax<int>(10, height / 25);
    closeButton.setBounds(width*0.75, height/40, buttonWidth, buttonHeight);

    list.updateContent();
    list.setBounds(center.x, center.y,width, height/1.5);
    list.setCentreRelative(0.5f, 0.5f);


}

void UsersSelect::changeListenerCallback(juce::ChangeBroadcaster* source) {

    this->listSelectedUser = model.modelSelectedUser;
    sendChangeMessage();

};