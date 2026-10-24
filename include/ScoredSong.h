#ifndef ATMOSPHERE_SCOREDSONG_H
#define ATMOSPHERE_SCOREDSONG_H
#include "Song.h"

struct ScoredSong {
    Song song;
    double score = 0.0;
};

#endif //ATMOSPHERE_SCOREDSONG_H