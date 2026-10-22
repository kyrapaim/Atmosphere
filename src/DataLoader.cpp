#include "DataLoader.h"
#include "Weather.h"
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <string>
#include <set>
using namespace std;

//called by loadSongs() and loadWeather() for each line, separating the CSV line into usable strings to make objects
vector<string> parseCSVLine(const string& line) {
    vector<string> row;
    string current; //temporarily stores chars being collected for one specific filed
    bool quotes = false; //tracks whether inside quotes or not so commas within quotes can be included

    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];

        if (c == '"') {
            if (quotes && i + 1 < line.size() && line[i + 1] == '"') {
                current += '"';
                i++;
            } else {
                quotes = !quotes;
            }
        } else if (c == ',' && !quotes) {
            //reached end of field, data gets pushed back
            row.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }

    //add final value after loop ends
    row.push_back(current);
    return row;
}


//opens file, reads each line, creates the Song objects using the CSV data and stores them in a vector
vector<Song> loadSongs(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Could not open file " + filename);
    }

    vector<Song> songs;
    string line;

    //read the first line of the CSV file, separate the column names
    getline(file, line);
    vector<string> headers = parseCSVLine(line);

    //map each column name (key) to the index of that column (value) so we only have to create one map for the column indexes of the entirety of the 100k songs
    unordered_map<string,size_t> columnIndex;

    for (size_t i = 0; i < headers.size(); i++) {
        columnIndex[headers[i]] = i;
    }

    //read remaining data
    while (getline(file, line)) {
        vector<string> songData = parseCSVLine(line);
        Song song;
        song.id = songData[columnIndex.at("id")];
        song.title = songData[columnIndex.at("title")];
        song.artist = songData[columnIndex.at("artist")];

        //strings to double values for music stats
        song.key = stod(songData[columnIndex.at("key")]);
        song.tempo = stod(songData[columnIndex.at("tempo")]);
        song.loudness = stod(songData[columnIndex.at("loudness")]);

        songs.push_back(song);
    }

    cout << "Successfully loaded " << songs.size() << " songs" << endl;
    return songs;
}


vector<WeatherProfile> loadWeather(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Could not open file " + filename);
    }

    vector<WeatherProfile> weathers;
    string line;

    getline(file, line);
    vector<string> headers = parseCSVLine(line);
    unordered_map<string,size_t> columnIndex;

    for (size_t i = 0; i < headers.size(); i++) {
        columnIndex[headers[i]] = i;
    }

    while (getline(file, line)) {
        vector<string> weatherData = parseCSVLine(line);
        WeatherProfile weatherProfile;

        weatherProfile.city = weatherData[columnIndex.at("Station.City")]; // using names directly used in the weather CSV file derived from CORGIS
        weatherProfile.state = weatherData[columnIndex.at("Station.State")];
        weatherProfile.month = weatherData[columnIndex.at("Date.Month")]; // only really care about the seasons, will take averages of the overall month


        weatherProfile.temperature = stod(weatherData[columnIndex.at("Data.Temperature.Avg Temp")]);
        weatherProfile.precipitation = stod(weatherData[columnIndex.at("Data.Precipitation")]);
        weatherProfile.windSpeed = stod(weatherData[columnIndex.at("Data.Wind.Speed")]);

        weathers.push_back(weatherProfile);
    }

    cout << "Successfully loaded " << weathers.size() << " weather profiles" << endl;
    return weathers;

}

vector<WeatherProfile> calculateMonthlyAverages(const vector<WeatherProfile>& weatherProfiles) {
    struct WeatherTotals {
        //for each city and each month, need to keep running total of data and how many reports were made that month (for averaging)
        WeatherProfile profile;
        int count = 0;
    };

    //grouping observations/reports by city, state, and month
    unordered_map<string,WeatherTotals> monthlyReports;
    //plural, key is city, value is WeatherTotals object that stores accumulated monthly data

    for (const WeatherProfile& weeklyReport : weatherProfiles) {
        string key = weeklyReport.state + "," + weeklyReport.city + "," + weeklyReport.month;
        //weeklyReport is ONE original weather observations

        WeatherTotals& monthlyReport = monthlyReports[key]; //SINGLE running total for each city's MONTH
        //if the key doesn't already exist, it will insert a new WeatherTotals object into the map
        //if it does, then it will retrieve the previous existing object and update that

        //no weekly observations accumulated yet, initialize
        if (monthlyReport.count == 0) {
            monthlyReport.profile.city = weeklyReport.city;
            monthlyReport.profile.state = weeklyReport.state;
            monthlyReport.profile.month = weeklyReport.month;
        }


        //accumulating totals for every report
        monthlyReport.profile.temperature += weeklyReport.temperature;
        monthlyReport.profile.precipitation += weeklyReport.precipitation;
        monthlyReport.profile.windSpeed += weeklyReport.windSpeed;

        //increase the number of observations we have seen for that specific city, state, and month so far
        monthlyReport.count++;

    }

    //calculating the averages
    vector<WeatherProfile> monthlyAverages;

    for (const auto& entry : monthlyReports) {
        //now for each accumulated monthly report, we need to average its stats with how many entries there were for that month, and then insert only ONE WeatherProfile object (for the month) into the vector
        const WeatherTotals& totalMonthReport = entry.second;

        WeatherProfile average = totalMonthReport.profile;

        average.temperature /= totalMonthReport.count;
        average.precipitation /= totalMonthReport.count;
        average.windSpeed /= totalMonthReport.count;

        monthlyAverages.push_back(average);
    }

    //returns WeatherProfile objects, but each profile is simplified/averaged to one city in an overall month
    return monthlyAverages;

}

vector<string> getAvailableCities(const vector<WeatherProfile>& monthlyAverages) {
    set<string> uniqueCities; //keeps city organizes in alphabetical order

    for (const WeatherProfile& profile : monthlyAverages) {
        string city = profile.city + ", " + profile.state;

        uniqueCities.insert(city);
    }

    vector<string> availableCities(uniqueCities.begin(), uniqueCities.end());
    return availableCities;
}

const WeatherProfile* findWeatherProfile(const vector<WeatherProfile>& monthlyAverages, const string& city, const string& state, const string& month) {
    for (const WeatherProfile& profile : monthlyAverages) {
        if (profile.city == city && profile.state == state && profile.month == month) {
            return &profile;
        }
    }

    return nullptr;
}