#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

namespace sideBarDataTypes {
	struct albumData {

		albumData(int thingy, juce::Image& image, std::string name) {
			this->album_id = thingy;
			this->album_art = image;
			this->album_name = name;
		}
		albumData() {};
		int album_id;
		juce::Image album_art;
		std::string album_name;

	};

	struct playlistData {

		playlistData(int id, std::string name) {

			this->playlist_id = id;
			this->playlist_name = name;

		}

		playlistData() {};

		int playlist_id;
		std::string playlist_name;

	};
};

class sideBarListModel : public juce::ListBoxModel,
						 public juce::ChangeBroadcaster				
{

	public:

		int selectedElementIndex;

		enum sideBarItemType {

			directory,
			playlist,
			albums

		};


		sideBarListModel();
		~sideBarListModel();
		int getNumRows() override;
		void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
		void listBoxItemDoubleClicked(int row, const juce::MouseEvent& mouse) override;
		void setDirectoryData(juce::Array<std::string> data);
		void setAlbumData(juce::Array<sideBarDataTypes::albumData> data);
		void setPlaylistData(juce::Array<sideBarDataTypes::playlistData> data);
		void changeModelType(sideBarItemType newModelType);
		std::string getDirectory(int index);
		sideBarDataTypes::albumData getAlbumData(int index);
		sideBarItemType getModelType();

	private:

		juce::Array<std::string> directory_data; 
		juce::Array<sideBarDataTypes::albumData> album_data;
		juce::Array<sideBarDataTypes::playlistData> playlist_data;
		short selectedItemIndex;
		sideBarItemType modelType;

};