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

}

void MaxHeap::extractMax(){

}

int getSize(){

}

vector<ScoredSong> extractMaxN(int MaxN){

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

        if(left < currentsize && songs[index].score < songs[left].score)
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
