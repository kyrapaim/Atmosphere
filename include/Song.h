// defines info each song stores
#ifndef ATMOSPHERE_SONG_H
#define ATMOSPHERE_SONG_H
#pragma once
#include <string>

struct Song {
    std::string id;
    std::string title;
    std::string artist;

    double key = -1.0;
    int mode = 0;
    double tempo = 0.0;
    double loudness = 0.0;
    double compatibility = 0.0;

};

#endif //ATMOSPHERE_SONG_H