#ifndef ATMOSPHERE_MERGESORT_H
#define ATMOSPHERE_MERGESORT_H
#include <vector>
#include <cstddef>
#include "ScoredSong.h"

void mergeSort(std::vector<ScoredSong>& songs);

std::vector<ScoredSong> topKMergeSort(
    std::vector<ScoredSong>& songs, std::size_t k);

#endif //ATMOSPHERE_MERGESORT_H