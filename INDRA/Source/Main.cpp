/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/
#ifdef NDEBUG

    #define FILEPATH true

#else

    #define FILEPATH false

#endif


#include <JuceHeader.h>
#include "MainComponent.h"
#include <fstream>
#include <string>

//==============================================================================
class INDRAApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    INDRAApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));

    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name) : DocumentWindow(name,
            juce::Colours::black,
            DocumentWindow::allButtons)
        {

            std::string filepath;
            if (!FILEPATH) {

                filepath = "../../../Storage/lastUser.txt";

            }
            else {

                filepath = "../../../../../../Storage/lastUser.txt";

            }

            std::string userIDstring = "-1";
            bool fileExists;
            {
                std::ifstream f(filepath);
                fileExists = f.good();

            }

            std::fstream userFile;

            if (fileExists) {

                userFile.open(filepath, std::fstream::in | std::fstream::out);
                std::getline(userFile, userIDstring);
                if (userIDstring == "") userIDstring = "-1";
            }
            else {

                userFile.open(filepath, std::fstream::in | std::fstream::out | std::fstream::trunc);

            }

            userFile.close();

            DBG(std::stoi(userIDstring));

            setBoundsRelative(0.25f, 0.25f, 0.8f, 0.8f);
            setResizable(true,false);
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(std::stoi(userIDstring)),false);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);

        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

    private:
        std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (INDRAApplication)
