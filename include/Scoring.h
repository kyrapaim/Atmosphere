// declares compatibility scoring function between songs and weather
#ifndef ATMOSPHERE_SCORING_H
#define ATMOSPHERE_SCORING_H
#include "Song.h"
#include "Weather.h"

double calculateScore(const Song& song, const WeatherProfile& weather);

#endif //ATMOSPHERE_SCORING_H