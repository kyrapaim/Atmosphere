//
// Created by kyrap on 10/20/2026.
//
#include "MaxHeap.h"
#include <vector>

using namespace std;


MaxHeap::MaxHeap(){

}

MaxHeap::MaxHeap(ScoredSong song){
    
}

MaxHeap::~MaxHeap(){

}

void MaxHeap::insert(ScoredSong song){
    songs.push_back(song);
    currentsize += 1;

    heapifyUp(currentsize - 1);

}

ScoredSong MaxHeap::extractMax(){
    ScoredSong max = songs[0]; // replace first element with last
    songs[0] = songs[currentsize - 1];
    songs.pop_back();
    currentsize -= 1;

    heapifyDown(0); // call heapifyDown on beginning of heap

    return max;

}

int MaxHeap::getSize(){
    return currentsize;

}

vector<ScoredSong> MaxHeap::extractMaxN(int MaxN){
    vector<ScoredSong> result {};

    for(int i = 0; i < MaxN; i++){
        result.push_back(extractMax());
    }

    return result;
}

void MaxHeap::heapifyUp(int index){
    while(index > 0){
        if(songs[index].score > songs[(index-1)/2].score){
            ScoredSong temp = songs[(index-1)/2];
            songs[(index - 1)/2] = songs[index];
            songs[index] = temp;
            index = (index -1)/2;
            continue;
        }
        break;
    }

}

void MaxHeap::heapifyDown(int index){

    while(index < currentsize){
        int left = index * 2 + 1;
        int right = index * 2 + 2;
        int largerI = index;

        if(left < currentsize && songs[largerI].score < songs[left].score)
            largerI = left;
        if(right < currentsize && songs[largerI].score < songs[right].score)
            largerI = right;
        if(largerI != index){
            ScoredSong Temp = songs[largerI];
            songs[largerI] = songs[index];
            songs[index] = Temp;
            index = largerI;
            continue;
        }
        break;

    }

}

