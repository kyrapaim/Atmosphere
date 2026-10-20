#include <iostream>
#include "DataLoader.h"

using namespace std;

int main() {

    try {
        vector<Song> songs =
            loadSongs("data/music_100k.csv");

        cout << "\nTotal songs: "
             << songs.size() << endl;

        // Display the first 5 songs
        for (size_t i = 0;
             i < 5 && i < songs.size(); i++) {

            cout << "\nSong " << i + 1 << endl;
            cout << "Title: " << songs[i].title << endl;
            cout << "Artist: " << songs[i].artist << endl;
            cout << "Key: " << songs[i].key << endl;
            cout << "Tempo: " << songs[i].tempo << endl;
            cout << "Loudness: " << songs[i].loudness << endl;
             }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
