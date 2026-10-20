#include "DataLoader.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

//called by loadSongs() and loadWeather() for each line, separating the CSV line into usable strings to make objects
vector<string> parseCSVLine(const string& line) {
    vector<string> columns;
    string current; //temporarily stores chars being collected for one specific filed
    bool quotes = false; //tracks whether inside quotes or not so commas within quotes can be included

    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];

        if (c == '"') {
            if (quotes && i + 1 < line.size() && line[i + 1] == '"') {
                current += '"';
                i++;
            } else {
                quotes = !quotes;
            }
        } else if (c == ',' && !quotes) {
            //reached end of field, data gets pushed back
            columns.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }

    //add final value after loop ends
    columns.push_back(current);
    return columns;
}


//opens file, reads each line, creates the Song objects using the CSV data and stores them in a vector
vector<Song> loadSongs(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Could not open file " + filename);
    }

    vector<Song> songs;
    string line;

    //read the first line of the CSV file, separate the column names
    getline(file, line);
    vector<string> headers = parseCSVLine(line);

    //map each column name (key) to the index of that column (value) so we only have to create one map for the column indexes of the entirety of the 100k songs
    unordered_map<string,size_t> columnIndex;

    for (size_t i = 0; i < headers.size(); i++) {
        columnIndex[headers[i]] = i;
    }

    //read remaining data
    while (getline(file, line)) {
        vector<string> songData = parseCSVLine(line);
        Song song;
        song.id = songData[columnIndex.at("id")];
        song.title = songData[columnIndex.at("title")];
        song.artist = songData[columnIndex.at("artist")];

        //strings to double values for music stats
        song.key = stod(songData[columnIndex.at("key")]);
        song.tempo = stod(songData[columnIndex.at("tempo")]);
        song.loudness = stod(songData[columnIndex.at("loudness")]);

        songs.push_back(song);
    }

    cout << "Successfully loaded " << songs.size() << " songs" << endl;
    return songs;
}
