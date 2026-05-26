#include "Mp3BoxModel.h"
#include "MainSongComponent.h"
#include "songFileInfo.h"
#include "SQLiteManager.h"

Mp3DisplayBoxModel::Mp3DisplayBoxModel() {



};

Mp3DisplayBoxModel::~Mp3DisplayBoxModel() {


};

int Mp3DisplayBoxModel::getNumRows() {

	return this->mp3Files.size();

};

void Mp3DisplayBoxModel::setMp3Files(juce::Array<juce::File> &newMp3Files) {

	this->mp3Files = newMp3Files;

}

juce::Component* Mp3DisplayBoxModel::refreshComponentForRow(int rowNumber, bool isRowSelected, juce::Component* existingComponentToUpdate) {

	MainSongComponent* row = static_cast<MainSongComponent*>(existingComponentToUpdate);

	if (rowNumber < getNumRows())
	{
		if (!row) row = new MainSongComponent;

		juce::AudioFormatManager formatManager;
		formatManager.registerBasicFormats();

		if (auto reader = formatManager.createReaderFor(mp3Files[rowNumber]))
		{
			int lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
			std::string lol = (lengthInSeconds % 60 < 10) ?
				std::to_string(lengthInSeconds / 60) + ":0" + std::to_string(lengthInSeconds % 60) :
				std::to_string(lengthInSeconds / 60) + ":" + std::to_string(lengthInSeconds % 60);
			row->changeLenght(lol);
			delete reader;
		}

		row->setInterceptsMouseClicks(false, false);

		songInfo::songInformation yo = songInfo::getSongInfo(mp3Files[rowNumber]);
		if (!yo.infoAvailable) return row;

		row->changeArtwork(yo.albumImage);
		row->changeArtist(yo.ArtistName);
		row->changeSongName(yo.SongTitle);
		row->changeAlbumName(yo.AlbumName);


		/* Update all properties of your custom component with the data for the current row  */
	}
	else
	{
		delete existingComponentToUpdate;
		row = nullptr;
	}

	return row;

};

void Mp3DisplayBoxModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) {

	if (rowNumber > getNumRows()) return;
};

void Mp3DisplayBoxModel::listBoxItemDoubleClicked(int row, const juce::MouseEvent& mouse) {

	this->selectedMP3 = row;
	sendChangeMessage();

}

int Mp3DisplayBoxModel::getSelectedMP3() {

	return this->selectedMP3;

}