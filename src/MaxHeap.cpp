#include "MaxHeap.h"
#include "Ranking.h"
#include <vector>

using namespace std;


MaxHeap::MaxHeap(){

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

    for(int i = 0; i < MaxN && currentsize > 0; i++){
        result.push_back(extractMax());
    }

    return result;
}

void MaxHeap::heapifyUp(int index){
    while(index > 0){
        if (ranksBefore(songs[index], songs[(index - 1) / 2])) {
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

        if(left < currentsize && ranksBefore(songs[left], songs[largerI]))
            largerI = left;

        if(right < currentsize && ranksBefore(songs[right], songs[largerI]))
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

