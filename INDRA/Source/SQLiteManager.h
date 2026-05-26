
#pragma once

#include <JuceHeader.h>
#include "sqlite3.h"
#include "songFileInfo.h"
#include "SideBarListModel.h"

//==============================================================================

enum SQLiteManagerBootMode {

    VisualStudio,
    ReleaseBuild,

};

struct User {

    User(int ide, std::string n, std::string p, std::string path) {

        this->id = ide;
        this->name = n;
        this->password = p;
        this->pathtoimage = path;

    }

    int id;
    std::string name;
    std::string password;
    std::string pathtoimage;

};

class SQLiteManager {

    public:

        SQLiteManager(SQLiteManagerBootMode bootMode);
        
        ~SQLiteManager();
        
        sqlite3* getDBObj();

        int createNewUser(std::string username, std::string password = "NULL", std::string pathToProfileImage = "NULL");

        int deleteUser(std::string username);

        std::vector<User> getUsers();

        int updateSongInformation(int userid, int songid, int timelistening = 0, int playammount = 0);

        int addSong(songInfo::songInformation song, std::string filePath, int userid, int songLength);

        int getSongLength(int songid);

        int getUserIDByOrder(int order);

        juce::Array<sideBarDataTypes::albumData> getAlbums();

        juce::Image getAlbumArt(int albumid);

        int removeSong();

        juce::Array<juce::File> getAlbum(int albumid);

        int addPlaylist();

        int removePlaylist();

        juce::Array<juce::File> getPlaylist();

        juce::Array<sideBarDataTypes::playlistData> getPlaylistForUser(int userid);

        int addDirectory(std::string filepath, int userid);

        int removeDirectory();

        juce::Array<std::string> getDirectories(int userid);

    private:

        sqlite3* DB;
        int rc;
        char* errorContainer;
        std::string SQL;
        SQLiteManagerBootMode bootMode;

};


