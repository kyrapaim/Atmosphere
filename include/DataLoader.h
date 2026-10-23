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

std::vector<WeatherProfile> calculateMonthlyAverages(const std::vector<WeatherProfile>& weatherProfiles);

std::vector<std::string> getAvailableCities(const std::vector<WeatherProfile>& monthlyAverages);




#endif //ATMOSPHERE_DATALOADER_H