#include <iostream>
#include "Scoring.h"
#include "DataLoader.h"
#include "ScoredSong.h"



using namespace std;

int main() {

    //all testing purposes, do not mind

    //testing songs being loaded correctly
    try {
        vector<Song> songs =
            loadSongs("data/music_100k.csv");

        cout << "\nTotal songs: "
             << songs.size() << endl;

        // Display the first 5 songs
        for (size_t i = 0;
             i < 5 && i < songs.size(); i++) {

            cout << "\nSong " << i + 1 << endl;
            cout << "Title: " << songs[i].title << endl;
            cout << "Artist: " << songs[i].artist << endl;
            cout << "Key: " << songs[i].key << endl;
            cout << "Tempo: " << songs[i].tempo << endl;
            cout << "Loudness: " << songs[i].loudness << endl;
             }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    cout << endl;


    // testing the loading of weather data
    vector<WeatherProfile> weathers = loadWeather("data/weather.csv");

    // checking how many weather profiles were loaded
    cout << "Total weather profiles: " << weathers.size() << endl;

    // printing the first 5 weather profiles
    for (size_t i = 0; i < 5 && i < weathers.size(); i++) {
        cout << "\nWeather Profile " << i + 1 << endl;

        cout << "City: " << weathers[i].city << endl;
        cout << "State: " << weathers[i].state << endl;
        cout << "Month: " << weathers[i].month << endl;
        cout << "Temperature: " << weathers[i].temperature << endl;
        cout << "Precipitation: " << weathers[i].precipitation << endl;
        cout << "Wind Speed: " << weathers[i].windSpeed << endl;

    }

    cout << endl;

    //testing monthly averages

    vector<WeatherProfile> weather = loadWeather("data/weather.csv");

    vector<WeatherProfile> monthlyAverages = calculateMonthlyAverages(weather);

    cout << "Weekly reports: " << weather.size() << endl;
    cout << "Monthly profiles: " << monthlyAverages.size() << endl;

    for (int i = 0; i < 5 && i < monthlyAverages.size(); i++) {
        const WeatherProfile& profile = monthlyAverages[i];

        cout << profile.city << ", " << profile.state
             << " - " << profile.month << endl;

        cout << "Temperature: " << profile.temperature << endl;
        cout << "Precipitation: " << profile.precipitation << endl;
        cout << "Wind Speed: " << profile.windSpeed << endl;
    }

    cout << endl;

    //testing the avaliable cities are unique
    vector<string> availableCities = getAvailableCities(monthlyAverages);

    cout << "Available Cities:" << endl;

    for (size_t i = 0; i < availableCities.size(); i++) {
        cout << i + 1 << ". " << availableCities[i] << endl;
    }


    cout << endl;

    //testing finding specfic weather profiles
    const WeatherProfile* selectedWeather = findWeatherProfile(monthlyAverages, "Gainesville", "Florida", "9");

    if (selectedWeather != nullptr) {
        cout << "Temperature: " << selectedWeather->temperature << endl;
        cout << "Precipitation: " << selectedWeather->precipitation << endl;
        cout << "Wind Speed: " << selectedWeather->windSpeed << endl;
    }
    else {
        cout << "Weather profile not found!" << endl;
    }

    cout << endl;

    //testing the score compatibility of the first 5 songs in the music csv file
    vector<Song> songs = loadSongs("data/music_100k.csv");


    cout << endl;

    if (selectedWeather != nullptr) {

        for (size_t i = 0; i < 5 && i < songs.size(); i++) {

            double score = calculateScore(songs[i], *selectedWeather);

            cout << songs[i].title
                 << " by " << songs[i].artist
                 << " | Score: " << score << "/100"
                 << endl;
        }
    }


    cout << endl;

    //testing ScoredSong objects against the same weather profile of gainesville in jan

    vector<ScoredSong> scoredSongs;

    for (const Song& song : songs) {
        double score = calculateScore(song, *selectedWeather);

        ScoredSong scoredSong;
        scoredSong.song = song;
        scoredSong.score = score;

        scoredSongs.push_back(scoredSong);
    }

    cout << "Total scored songs: " << scoredSongs.size() << endl;

    for (int i = 50; i < 1000 && i < scoredSongs.size(); i++) {
        if (scoredSongs[i].score < 90) {
            continue;
        }
        cout << scoredSongs[i].song.title
             << " by " << scoredSongs[i].song.artist
             << " | Score: " << scoredSongs[i].score
             << endl;
    }

    return 0;



}
