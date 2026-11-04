cmake_minimum_required(VERSION 4.0)
project(Atmosphere)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Shared project code
add_library(AtmosphereCore
        src/DataLoader.cpp
        src/Scoring.cpp
        src/MergeSort.cpp
        src/MaxHeap.cpp
        include/ScoredSong.h
        include/Ranking.h
)

target_include_directories(AtmosphereCore PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Existing console application
add_executable(Atmosphere
        src/main.cpp
)

target_link_libraries(Atmosphere PRIVATE AtmosphereCore)

# New web server application
add_executable(AtmosphereServer
        src/server.cpp
)

target_link_libraries(AtmosphereServer PRIVATE AtmosphereCore)

if(WIN32)
    target_link_libraries(AtmosphereServer PRIVATE ws2_32)
endif()

# Avoid oversized MinGW debug information for the HTTP server
if(MINGW)
    target_compile_options(AtmosphereServer PRIVATE -g0)
endif()