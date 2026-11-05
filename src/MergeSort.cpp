#include "MergeSort.h"
#include "Ranking.h"
#include <cmath>
#include<stdexcept>
#include <algorithm>
using namespace std;

namespace {
    void mergeRanges(
        vector<ScoredSong>& songs,
        vector<ScoredSong>& temporary,
        size_t left,
        size_t mid,
        size_t right
    ) {
        size_t i = left;
        size_t j = mid;
        size_t out = left;

        while (i < mid && j < right) {
            if (ranksBefore(songs[j], songs[i])) {
                temporary[out++] = songs[j++];
            } else {
                temporary[out++] = songs[i++];
            }
        }

        // Copy any remaining records
        while (i < mid) {
            temporary[out++] = songs[i++];
        }

        while (j < right) {
            temporary[out++] = songs[j++];
        }

        // Copy the merged range back
        for (size_t index = left; index < right; ++index) {
            songs[index] = temporary[index];
        }
    }

    void sortRange(vector<ScoredSong>& songs, vector<ScoredSong>& temporary, size_t left, size_t right) {
        if (right - left < 2) return;
        size_t mid = left + (right - left) / 2;
        sortRange(songs, temporary, left, mid);
        sortRange(songs, temporary, mid, right);
        mergeRanges(songs, temporary, left, mid, right);
    }
}

void mergeSort(vector<ScoredSong>& songs) {
    for (const ScoredSong& entry : songs) {
        if (!isfinite(entry.score)) {
            throw invalid_argument("Song scores must be finite");
        }
    }
    if (songs.size() < 2) return;

    vector<ScoredSong> temporary(songs.size());

    sortRange(songs, temporary, 0, songs.size());

}

vector<ScoredSong> topKMergeSort(vector<ScoredSong>& songs, size_t k) {
    if (k > songs.size()) throw invalid_argument("K exceeds song size");
    if (k == 0) return {};

    mergeSort(songs);
    return vector<ScoredSong>(songs.begin(), songs.begin() + k);
}

