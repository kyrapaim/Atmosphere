// weather info used for recs
#ifndef ATMOSPHERE_WEATHER_H
#define ATMOSPHERE_WEATHER_H
#pragma once
#include <string>

struct WeatherProfile {
    std::string city;
    std::string date;

    int temperature = 0;
    double precipitation = 0.0;
    double windSpeed = 0.0;
};
#endif //ATMOSPHERE_WEATHER_H