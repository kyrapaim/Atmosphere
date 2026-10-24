#pragma once
#include "Scoring.h"
#include <algorithm>
#include <cmath>
using namespace std;

// Keeps a value between the minimum and maximum
double clampValue(double value, double minimum, double maximum) {
    return max(minimum, min(value, maximum));
}

double calculateScore(const Song& song, const WeatherProfile& weather) {

    // normalizing the weather data, 32 as a min, 100 as a max
    double temperature = clampValue((weather.temperature - 32.0) / 68.0,0.0, 1.0);

    double rain = clampValue(weather.precipitation / 1.0,0.0, 1.0);

    // calculate target tempo based on conditions (ranging from 70 to 140 BPM)
    double targetTempo = 75.0 + 65.0 *(0.65 * temperature +0.35 * (1.0 - rain));

    //same with loudness (ranging from -16 to -4 dB
    double targetLoudness = -16.0 + 12.0 *(0.70 * temperature + 0.30 * (1.0 - rain));

    //pretty much equal for major and minor keys
    double majorPreference = clampValue(0.20 + 0.50 * temperature - 0.30 * rain,0.10, 0.90);

    //compatibility scores for these INDIVIDUAL metrics
    double tempoScore = clampValue(1.0 - abs(song.tempo - targetTempo) / 80.0,0.0, 1.0);

    double loudnessScore = clampValue(1.0 - abs(song.loudness - targetLoudness) / 20.0,0.0, 1.0);

    double modeScore = 0.5;

    if (song.mode == 1) {
        modeScore = majorPreference;
    }
    else if (song.mode == 0) {
        modeScore = 1.0 - majorPreference;
    }

    //getting final score by combining these elements, tempo has the heaviest weight, then loudness, then key
    double finalScore = 100.0 * (0.45 * tempoScore + 0.35 * loudnessScore + 0.20 * modeScore);

    return finalScore;
}

