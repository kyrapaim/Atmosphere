//
// Created by Yuxuan Tao on 11/3/26.
//

#ifndef ATMOSPHERE_RANKING_H
#define ATMOSPHERE_RANKING_H

#include "ScoredSong.h"

inline bool ranksBefore(const ScoredSong& a, const ScoredSong& b) {
    if (a.score != b.score) {
        return a.score > b.score;
    }
    return a.song.id < b.song.id;
}
#endif //ATMOSPHERE_RANKING_H
