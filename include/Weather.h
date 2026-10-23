// weather info used for recs
#ifndef ATMOSPHERE_WEATHER_H
#define ATMOSPHERE_WEATHER_H
#pragma once
#include <string>
#include <vector>

struct WeatherProfile {
    std::string city;
    std::string state;
    std::string month;

    double temperature = 0.0;
    double precipitation = 0.0;
    double windSpeed = 0.0;
};

const WeatherProfile* findWeatherProfile(const std::vector<WeatherProfile>& monthlyAverages, const std::string& city, const std::string& state, const std::string& month);


#endif //ATMOSPHERE_WEATHER_H