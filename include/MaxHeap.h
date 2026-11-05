#ifndef ATMOSPHERE_MAXHEAP_H
#define ATMOSPHERE_MAXHEAP_H
#include <vector>
#include "ScoredSong.h"
using namespace std;


class MaxHeap{
    public:
        MaxHeap();
        MaxHeap(ScoredSong song);
        ~ MaxHeap();

        void insert(ScoredSong song);
        ScoredSong extractMax();

        int getSize();

        vector<ScoredSong> extractMaxN(int maxN);

    private:
        int currentsize = 0;
        vector<ScoredSong> songs{};

        void heapifyUp(int index);
        void heapifyDown(int index);

};

#endif //ATMOSPHERE_MAXHEAP_H
