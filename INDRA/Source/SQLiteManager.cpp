
#include <JuceHeader.h>
#include <string>
#include <vector>
#include <cstring>
#include "SQLiteManager.h"
#include "sqlite3.h"
#include "songFileInfo.h"

SQLiteManager::SQLiteManager(SQLiteManagerBootMode bootMode) {

    std::string DatabasePath;

    switch (bootMode) {

        case ReleaseBuild: { DatabasePath = "../../../../../../Storage/RUDIMENT.db"; break; }
        case VisualStudio: { DatabasePath = "../../../Storage/RUDIMENT.db"; break; }

    }

    rc = sqlite3_open(DatabasePath.c_str(), &(this->DB));

    if (rc) {

        DBG(sqlite3_errmsg(this->DB));

    }

    this->bootMode = bootMode;

    // SETUP DB IN CASE IT DOESN'T EXIST

    SQL =
        "CREATE TABLE IF NOT EXISTS uporabnik ("
        "UporabnikID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "Ime VARCHAR(255) UNIQUE NOT NULL,"
        "Geslo VARCHAR(255) DEFAULT NULL,"
        "NaslovUporabniskeSlike VARCHAR(255) DEFAULT NULL);"
        " "
        "CREATE TABLE IF NOT EXISTS album ("
        "AlbumID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "AlbumIme VARCHAR(255) NOT NULL,"
        "AlbumUmetnik VARCHAR(255) NOT NULL,"
        "AlbumDatumIzdaje DATE NOT NULL);"
        " "
        "CREATE TABLE IF NOT EXISTS pesem ("
        "PesemID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "PesemIme VARCHAR(255) NOT NULL,"
        "NaslovPesmi VARCHAR(255) NOT NULL,"
        "DolzinaSekunde INTEGER NOT NULL,"
        "AlbumID INTEGER,"
        "FOREIGN KEY (AlbumID) REFERENCES album(AlbumID));"
        " "
        "CREATE TABLE IF NOT EXISTS playlista("
        "PlaylistaID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "PlaylistaIme VARCHAR(255) NOT NULL,"
        "SteviloPesmi SMALLINT NOT NULL DEFAULT 0,"
        "NaslovPlaylistSlike VARCHAR(255),"
        "Opis VARCHAR(255),"
        "UporabnikID INTEGER,"
        "FOREIGN KEY (UporabnikID) REFERENCES uporabnik(UporabnikID)"
        "ON DELETE CASCADE); "
        " "
        "CREATE TABLE IF NOT EXISTS vsebina_playliste("
        "PlaylistaID INTEGER,"
        "UporabnikID INTEGER,"
        "PesemID INTEGER,"
        "AlbumID INTEGER,"
        "FOREIGN KEY (PlaylistaID) REFERENCES playlista(PlaylistaID) ON DELETE CASCADE,"
        "FOREIGN KEY (UporabnikID) REFERENCES playlista(UporabnikID) ON DELETE CASCADE,"
        "FOREIGN KEY (PesemID) REFERENCES pesem(PesemID) ON DELETE CASCADE,"
        "FOREIGN KEY (AlbumID) REFERENCES pesem(AlbumID));"
        " "
        "CREATE TABLE IF NOT EXISTS uporabnik_ima_pesem("
        "UporabnikID INTEGER,"
        "PesemID INTEGER,"
        "AlbumID INTEGER,"
        "CasPosusanjaSekunde TINYINT,"
        "KolicinaPredvajanj TINYINT NOT NULL DEFAULT 0,"
        "FOREIGN KEY (UporabnikID) REFERENCES uporabnik(UporabnikID) ON DELETE CASCADE,"
        "FOREIGN KEY (PesemID) REFERENCES pesem(PesemID) ON DELETE CASCADE,"
        "FOREIGN KEY (AlbumID) REFERENCES pesem(AlbumID));"
        ""
        "CREATE TABLE IF NOT EXISTS direktorija("
        "DirektorijaID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "direktorija VARCHAR(255),"
        "UporabnikID INT,"
        "FOREIGN KEY (UporabnikID) REFERENCES uporabnik(UporabnikID) ON DELETE CASCADE);";

    rc = sqlite3_exec(this->DB, SQL.c_str(), NULL, NULL, &errorContainer);
    if (rc != SQLITE_OK) {
        DBG(errorContainer);
    }

};


SQLiteManager::~SQLiteManager() {

    sqlite3_close(this->DB);
    
};


sqlite3* SQLiteManager::getDBObj() {

    return this->DB;

}

int SQLiteManager::createNewUser(std::string username, std::string password, std::string pathToProfileImage) {

    int returnValue;
    
    this->SQL = "INSERT INTO uporabnik(Ime, Geslo, NaslovUporabniskeSlike) "
                "VALUES('" + username + "','" + password + "','" + pathToProfileImage + "');";

    this->rc = sqlite3_exec(this->DB, SQL.c_str(), NULL, NULL, &(this->errorContainer));
    returnValue = 0;

    if (this->rc != SQLITE_OK) {

        DBG(errorContainer);
        returnValue = -1;
    }

    return returnValue;
}

int SQLiteManager::deleteUser(std::string username) {

    int returnValue;

    this->SQL = "DELETE FROM uporabnik WHERE uporabnik.Ime = '" + username + "';";

    this->rc = sqlite3_exec(this->DB, SQL.c_str(), NULL, NULL, &(this->errorContainer));
    returnValue = 0;

    if (this->rc != SQLITE_OK) {

        DBG(errorContainer);
        returnValue = -1;
    }

    return returnValue;

}

std::vector<User> SQLiteManager::getUsers() {
    
    std::vector<User> returnVector;

    this->SQL = "SELECT * FROM uporabnik;";
    sqlite3_stmt* stmt;

    this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        DBG(sqlite3_errmsg(this->DB));
        sqlite3_finalize(stmt);
        return returnVector;
    }

    while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {
      
        int idee = sqlite3_column_int(stmt, 0);
        std::string name = (char*)sqlite3_column_text(stmt, 1);
        std::string password = (char*)sqlite3_column_text(stmt, 2);
        std::string path = (char*)sqlite3_column_text(stmt, 3);

        returnVector.push_back(User(idee,name, password, path));

    }

    if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
        DBG("SQLite ERROR.");
        DBG(std::to_string(this->rc));
    }

    sqlite3_finalize(stmt);

    return returnVector;
}

int SQLiteManager::updateSongInformation(int userid, int songid, int timelistening, int playammount) {

    int returnValue;

    this->SQL = "UPDATE uporabnik_ima_pesem "
        "SET casposusanjasekunde = casposusanjasekunde + "+ std::to_string(timelistening) + ","
        "kolicinapredvajanj = kolicinapredvajanj + "+ std::to_string(playammount) +","
        "WHERE uporabnikid = " + std::to_string(userid) + " AND pesemid = " + std::to_string(songid) + "; ";

    this->rc = sqlite3_exec(this->DB, SQL.c_str(), NULL, NULL, &(this->errorContainer));
    returnValue = 0;
    
    if (this->rc != SQLITE_OK) {

        DBG(errorContainer);
        returnValue = -1;
    }

    return returnValue;

}

int SQLiteManager::addSong(songInfo::songInformation song, std::string filePath, int userID, int songLength) {

    int returnValue = 0;
    sqlite3_stmt* stmt;

    /* TODO:
    
        - CHECK IF SONG ALREADY EXISTS (has same name, same album and same artist)
            - if it does, check if user has it, if he doesn't add it to current user, else return 0
        - IF IT DOESN'T EXIST, ADD ALBUM THEN ADD SONG, ADD USER OWNERSHIP
    
    */

    // Poglej èe pesem že obstaja

    this->SQL = "SELECT * FROM pesem "
        "JOIN album USING (AlbumID) "
        "WHERE PesemIme = '" + song.SongTitle + "' AND AlbumIme = '" + song.AlbumName + "' AND AlbumUmetnik = '" + song.ArtistName + "';";

    this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
    if (this->rc != SQLITE_OK) {
        DBG(sqlite3_errmsg(this->DB));
        sqlite3_finalize(stmt);
        returnValue = 1;
        return returnValue;
    }

    short count = 0;
    int songID = 0;
    int albumID = 0;

    while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        count++;
        songID = sqlite3_column_int(stmt, 0);
        albumID = sqlite3_column_int(stmt, 4);

    }

    if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
        DBG("SQLite ERROR.");
        DBG(std::to_string(rc));
        returnValue = 1;
        return returnValue;
    }

    bool userHasSong = false;
    this->rc=sqlite3_finalize(stmt);

    if (rc != SQLITE_OK) {
        DBG(sqlite3_errmsg(this->DB));
        sqlite3_finalize(stmt);
        returnValue = 1;
        return returnValue;
    }

    if (count != 0) {

        // Pesem obstaja, preveri èe jo ima uporabnik.

        this->SQL = "SELECT PesemID FROM uporabnik_ima_pesem "
            "WHERE PesemID = "+ std::to_string(songID) +" AND UporabnikID = " + std::to_string(userID) + "; ";

        this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);

        if (this->rc != SQLITE_OK) {
            DBG(sqlite3_errmsg(this->DB));
            sqlite3_finalize(stmt);
            returnValue = 1;
            return returnValue;
        }

        int song_user_count = 0;

        while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

            song_user_count++;

        }

        if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
            DBG("SQLite ERROR.");
            DBG(std::to_string(rc));
            returnValue = 1;
            return returnValue;
        }

        if (song_user_count != 0) userHasSong = true;

        sqlite3_finalize(stmt);
    }
    else {

        // Pesem ne obstaja, preveri èe obstaja album


        this->SQL = "SELECT * FROM album WHERE AlbumIme = '" + song.AlbumName + "' AND AlbumUmetnik = '" + song.ArtistName + "'; ";

        this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);

        if(this->rc != SQLITE_OK) {
            DBG(sqlite3_errmsg(this->DB));
            sqlite3_finalize(stmt);
            returnValue = 1;
            return returnValue;
        }

        bool albumExists = false;
        int step = (this->rc = sqlite3_step(stmt));

        if (step == SQLITE_ROW || step == SQLITE_ROW) albumExists = true;

        if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
            DBG("SQLite ERROR.");
            DBG(std::to_string(rc));
            returnValue = 1;
            return returnValue;
        }

        sqlite3_finalize(stmt);

        // Ce album ne obstaja ga naredi

        if (!albumExists)
        {

            this->SQL = "INSERT INTO album(AlbumIme,AlbumUmetnik,AlbumDatumIzdaje)"
                "VALUES('" + song.AlbumName + "','" + song.ArtistName + "'," + song.trackYear + ")";
            
            this->rc = sqlite3_exec(this->DB, SQL.c_str(), NULL, NULL, &(this->errorContainer));
            returnValue = 0;

            if (this->rc != SQLITE_OK) {

                DBG(errorContainer);
                returnValue = -1;
                return returnValue;
            }

        }

        // pridobi se albumID za insert
        this->SQL = "SELECT AlbumID FROM album WHERE AlbumIme = '" + song.AlbumName + "' AND AlbumUmetnik"
            " = '" + song.ArtistName + "';";

        this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            DBG(sqlite3_errmsg(this->DB));
            sqlite3_finalize(stmt);
            returnValue = -1;
            return returnValue;
        }

        while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

            albumID = sqlite3_column_int(stmt, 0);

        }

        if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
            DBG("SQLite ERROR.");
            DBG(std::to_string(this->rc));
            returnValue = -1;
        }

        sqlite3_finalize(stmt);
        
        this->SQL = "INSERT INTO pesem(PesemIme,NaslovPesmi,DolzinaSekunde,AlbumID) "
            "VALUES ('" + song.SongTitle + "','" + filePath + "'," + std::to_string(songLength) + "," + std::to_string(albumID) + ");";

        this->rc = sqlite3_exec(this->DB, SQL.c_str(), NULL, NULL, &(this->errorContainer));
        returnValue = 0;

        if (this->rc != SQLITE_OK) {

            DBG(errorContainer);
            returnValue = -1;
            return returnValue;
        }
        
        // se song ID
        this->SQL = "SELECT PesemID FROM pesem WHERE PesemIme = '" + song.SongTitle + "' AND AlbumID"
            " = '" + std::to_string(albumID) + "';";

        this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            DBG(sqlite3_errmsg(this->DB));
            sqlite3_finalize(stmt);
            returnValue = -1;
            return returnValue;
        }

        while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

            songID = sqlite3_column_int(stmt, 0);

        }

        if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
            DBG("SQLite ERROR.");
            DBG(std::to_string(this->rc));
            returnValue = -1;
        }

        sqlite3_finalize(stmt);

    }

    if (userHasSong == false) {

        this->SQL = "INSERT INTO uporabnik_ima_pesem(UporabnikID,PesemID,AlbumID,CasPosusanjaSekunde,KolicinaPredvajanj) "
            "VALUES (" + std::to_string(userID) + "," + std::to_string(songID) + ","+ std::to_string(albumID) + ",0,1);";

        this->rc = sqlite3_exec(this->DB, SQL.c_str(), NULL, NULL, &(this->errorContainer));
        returnValue = 0;

        if (this->rc != SQLITE_OK) {

            DBG(errorContainer);
            returnValue = -1;
            return returnValue;
        }

    }

    return returnValue;

}

int SQLiteManager::getSongLength(int songid = -1) {

    return -1;
}

int SQLiteManager::getUserIDByOrder(int order) {

    int userid = -1;

    this->SQL = "SELECT UporabnikID FROM uporabnik ORDER BY UporabnikID LIMIT "
                + std::to_string(order) + ", 1";
    sqlite3_stmt* stmt;

    this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        DBG(sqlite3_errmsg(this->DB));
        sqlite3_finalize(stmt);
        return userid;
    }

    while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        userid = sqlite3_column_int(stmt, 0);

    }

    if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
        DBG("SQLite ERROR.");
        DBG(std::to_string(this->rc));
    }

    sqlite3_finalize(stmt);


    return userid;
}

juce::Array<std::string> SQLiteManager::getDirectories(int userid) {

    juce::Array<std::string> returnArray;
    sqlite3_stmt* stmt;

    this->SQL = "SELECT direktorija FROM direktorija WHERE UporabnikID = " + std::to_string(userid) + ";";

    this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        DBG(sqlite3_errmsg(this->DB));
        sqlite3_finalize(stmt);
    }

    while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        returnArray.add((char*)sqlite3_column_text(stmt, 0));

    }

    if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
        DBG("SQLite ERROR.");
        DBG(std::to_string(this->rc));
    }

    sqlite3_finalize(stmt);

    return returnArray;
};

juce::Image SQLiteManager::getAlbumArt(int albumid) {

    // dobi naslov prve pesmi na albumu

    sqlite3_stmt* stmt;
    std::string filepath;
    juce::Image returnImage;

    this->SQL = "SELECT NaslovPesmi FROM pesem WHERE AlbumID = "+std::to_string(albumid)+" LIMIT 1 ;";

    this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        DBG(sqlite3_errmsg(this->DB));
        sqlite3_finalize(stmt);
    }

    while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        filepath = (char*)sqlite3_column_text(stmt, 0);

    }

    if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
        DBG("SQLite ERROR.");
        DBG(std::to_string(this->rc));
    }

    sqlite3_finalize(stmt);


    //ustvar file pol pa iz njega pridob sliko

    juce::File temp(filepath);
    songInfo::songInformation temp_info = songInfo::getSongInfo(temp);

    returnImage = temp_info.albumImage;


    return returnImage;

};

juce::Array<sideBarDataTypes::albumData> SQLiteManager::getAlbums() {

    juce::Array<sideBarDataTypes::albumData> returnArray;
    sqlite3_stmt* stmt;

    this->SQL = "SELECT AlbumID, AlbumIme FROM album;";

    this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        DBG(sqlite3_errmsg(this->DB));
        sqlite3_finalize(stmt);
    }

    while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        int id = sqlite3_column_int(stmt, 0);
        std::string ime = (char*)sqlite3_column_text(stmt, 1);
        sideBarDataTypes::albumData da;
        da.album_id = id;
        da.album_art = getAlbumArt(id);
        da.album_name = ime;
        returnArray.add(da);
    }

    if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
        DBG("SQLite ERROR.");
        DBG(std::to_string(this->rc));
    }

    sqlite3_finalize(stmt);

    return returnArray;

};

juce::Array<sideBarDataTypes::playlistData> SQLiteManager::getPlaylistForUser(int userid) {

    juce::Array<sideBarDataTypes::playlistData> returnArray;
    sqlite3_stmt* stmt;

    this->SQL = "SELECT PlaylistaID, PlaylistaIme FROM playlista WHERE UporabnikID = "+std::to_string(userid) + ";";

    this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        DBG(sqlite3_errmsg(this->DB));
        sqlite3_finalize(stmt);
    }

    while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        sideBarDataTypes::playlistData yo;
        returnArray.add(yo);
    }

    if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
        DBG("SQLite ERROR.");
        DBG(std::to_string(this->rc));
    }

    sqlite3_finalize(stmt);

    return returnArray;


};

juce::Array<juce::File> SQLiteManager::getAlbum(int albumid) {

    juce::Array<juce::File> returnArray;
    sqlite3_stmt* stmt;

    this->SQL = "SELECT NaslovPesmi FROM pesem WHERE AlbumID = "+std::to_string(albumid)+"; ";

    this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        DBG(sqlite3_errmsg(this->DB));
        sqlite3_finalize(stmt);
    }

    while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        std::string naslov = (char*)sqlite3_column_text(stmt, 0);
        juce::File temp(naslov);
        returnArray.add(temp);

    }

    if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
        DBG("SQLite ERROR.");
        DBG(std::to_string(this->rc));
    }

    sqlite3_finalize(stmt);

    return returnArray;

};

int SQLiteManager::addDirectory(std::string filepath, int userid) {

    // poglej če obstaja

    int returnValue = 0;
    sqlite3_stmt* stmt;

    this->SQL = "SELECT * FROM direktorija WHERE UporabnikID = " + std::to_string(userid)+" "
                "AND direktorija = '" + filepath + "';";
    this->rc = sqlite3_prepare_v2(this->DB, this->SQL.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        DBG(sqlite3_errmsg(this->DB));
        sqlite3_finalize(stmt);
        returnValue = -1;
        return returnValue;
    }

    short count = 0;

    while ((this->rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        count++;

    }

    if (this->rc != SQLITE_DONE && this->rc != SQLITE_ROW) {
        DBG("SQLite ERROR.");
        DBG(std::to_string(this->rc));
        returnValue = -1;
        return returnValue;
    }

    sqlite3_finalize(stmt);

    if (count == 0) {

        this->SQL = "INSERT INTO direktorija(direktorija,UporabnikID) VALUES('" + filepath + "'," + std::to_string(userid) + ");";

        this->rc = sqlite3_exec(this->DB, SQL.c_str(), NULL, NULL, &(this->errorContainer));
        returnValue = 0;

        if (this->rc != SQLITE_OK) {

            DBG(errorContainer);
            returnValue = -1;
            return returnValue;
        }

    }

    return returnValue;
};