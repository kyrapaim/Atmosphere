// functions for loading the datasets
#ifndef ATMOSPHERE_DATALOADER_H
#define ATMOSPHERE_DATALOADER_H
#pragma once
#include <string>
#include <vector>
#include "Song.h"
#include "Weather.h"

std::vector<Song> loadSongs(const std::string& fileName);

std::vector<WeatherProfile> loadWeather(const std::string& fileName);


#endif //ATMOSPHERE_DATALOADER_H