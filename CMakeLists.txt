cmake_minimum_required(VERSION 4.0)
project(Atmosphere)

set(CMAKE_CXX_STANDARD 20)

add_executable(Atmosphere src/main.cpp
        include/Song.h
        include/Weather.h
        include/DataLoader.h
        include/Scoring.h
        include/MergeSort.h
        include/MaxHeap.h
        src/DataLoader.cpp
        src/MaxHeap.cpp
        src/MergeSort.cpp
        src/Scoring.cpp
)
