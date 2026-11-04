#include "httplib.h"
#include "json.hpp"
#include "DataLoader.h"
#include "Scoring.h"
#include "ScoredSong.h"
#include "MergeSort.h"
#include "MaxHeap.h"
#include "Weather.h"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

using json = nlohmann::json;

string monthToNumber(const string& month) {
    const vector<string> months = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
    };

    for (size_t i = 0; i < months.size(); i++) {
        if (months[i] == month) {
            return to_string(i + 1);
        }
    }

    throw invalid_argument("Invalid month: " + month);
}


int main() {
    namespace fs = std::filesystem;

    fs::path dataDirectory = fs::current_path() / "data";

    if (!fs::exists(dataDirectory)) {
        dataDirectory = fs::current_path().parent_path() / "data";
    }

    std::string songsPath =
        (dataDirectory / "music_100k.csv").string();

    std::string weatherPath =
        (dataDirectory / "weather.csv").string();

    std::vector<Song> songs = loadSongs(songsPath);

    std::vector<WeatherProfile> weatherReports =
        loadWeather(weatherPath);

    std::vector<WeatherProfile> monthlyWeather =
        calculateMonthlyAverages(weatherReports);

    std::cout << "Loaded " << songs.size()
              << " songs" << std::endl;

    std::cout << "Loaded " << monthlyWeather.size()
              << " monthly weather profiles" << std::endl;

    httplib::Server server;


    server.set_pre_routing_handler(
        [](const httplib::Request&,
           httplib::Response& response) {

            response.set_header(
                "Access-Control-Allow-Origin", "*"
            );

            response.set_header(
                "Access-Control-Allow-Methods",
                "GET, POST, OPTIONS"
            );

            response.set_header(
                "Access-Control-Allow-Headers",
                "Content-Type"
            );

            return httplib::Server::HandlerResponse::Unhandled;
        }
    );

    server.Options(
        R"(/.*)",
        [](const httplib::Request&,
           httplib::Response& response) {
            response.status = 204;
        }
    );


    server.Get("/data-status",
    [&songs, &monthlyWeather](
        const httplib::Request&,
        httplib::Response& response
    ) {
        json result = {
            {"songsLoaded", songs.size()},
            {"weatherProfilesLoaded", monthlyWeather.size()}
        };

        response.set_content(
            result.dump(),
            "application/json"
        );
    }
);

    server.Get("/health",
        [](const httplib::Request& request,
           httplib::Response& response) {

            json result = {
                {"status", "ok"},
                {"message", "Atmosphere backend is running"}
            };

            response.set_content(
                result.dump(),
                "application/json"
            );
        }
    );


server.Post("/recommend",
    [&songs, &monthlyWeather](
        const httplib::Request& request,
        httplib::Response& response
    ) {
        try {
            json input = json::parse(request.body);

            if (!input.is_object() ||
                !input.contains("city") ||
                !input.contains("month") ||
                !input.contains("count") ||
                !input.contains("algorithm") ||
                !input["city"].is_string() ||
                !input["month"].is_string() ||
                !input["count"].is_number_integer() ||
                !input["algorithm"].is_string()) {
                response.status = 400;
                response.set_content(
                    R"({"error":"Invalid request"})",
                    "application/json"
                );
                return;
            }

            string selectedCity = input["city"].get<string>();
            string month = input["month"].get<string>();
            int count = input["count"].get<int>();
            string algorithm = input["algorithm"].get<string>();

            if (count < 1 || count > 10000 ||
                (algorithm != "merge" && algorithm != "heap")) {
                response.status = 400;
                response.set_content(
                    R"({"error":"Invalid count or algorithm"})",
                    "application/json"
                );
                return;
            }

            // Frontend sends "Gainesville, Florida".
            size_t comma = selectedCity.rfind(',');

            if (comma == string::npos) {
                response.status = 400;
                response.set_content(
                    R"({"error":"Invalid city format"})",
                    "application/json"
                );
                return;
            }

            string city = selectedCity.substr(0, comma);
            string state = selectedCity.substr(comma + 1);

            // Remove leading/trailing spaces.
            auto trim = [](string& value) {
                size_t first = value.find_first_not_of(" \t\r\n");

                if (first == string::npos) {
                    value.clear();
                    return;
                }

                size_t last = value.find_last_not_of(" \t\r\n");
                value = value.substr(first, last - first + 1);
            };

            trim(city);
            trim(state);

            // CSV stores months as numbers.
            string numericMonth = monthToNumber(month);

            const WeatherProfile* weather =
                findWeatherProfile(
                    monthlyWeather,
                    city,
                    state,
                    numericMonth
                );

            if (weather == nullptr) {
                response.status = 404;
                response.set_content(
                    json({
                        {"error", "Weather profile not found"},
                        {"city", city},
                        {"state", state},
                        {"month", numericMonth}
                    }).dump(),
                    "application/json"
                );
                return;
            }

            // Calculate compatibility for every song.
            vector<ScoredSong> scoredSongs;
            scoredSongs.reserve(songs.size());

            for (const Song& song : songs) {
                double score = calculateScore(song, *weather);
                scoredSongs.push_back({song, score});
            }

            int actualCount = min(
                count,
                static_cast<int>(scoredSongs.size())
            );

            vector<ScoredSong> recommendations;

            auto start = chrono::steady_clock::now();

            if (algorithm == "merge") {
                recommendations = topKMergeSort(
                    scoredSongs,
                    static_cast<size_t>(actualCount)
                );
            } else {
                MaxHeap heap;

                for (const ScoredSong& song : scoredSongs) {
                    heap.insert(song);
                }

                recommendations = heap.extractMaxN(actualCount);
            }

            auto end = chrono::steady_clock::now();

            double elapsedMs =
                chrono::duration<double, milli>(
                    end - start
                ).count();

            json results = json::array();

            for (const ScoredSong& entry : recommendations) {
                results.push_back({
                    {"id", entry.song.id},
                    {"title", entry.song.title},
                    {"artist", entry.song.artist},
                    {"score", entry.score}
                });
            }

            json output = {
                {"city", selectedCity},
                {"month", month},
                {"algorithm", algorithm},
                {"requestedCount", count},
                {"returnedCount", recommendations.size()},
                {"executionTimeMs", elapsedMs},
                {"weather", {
                    {"temperature", weather->temperature},
                    {"precipitation", weather->precipitation},
                    {"windSpeed", weather->windSpeed}
                }},
                {"songs", results}
            };

            response.set_content(
                output.dump(),
                "application/json"
            );

        } catch (const exception& error) {
            response.status = 400;
            response.set_content(
                json({{"error", error.what()}}).dump(),
                "application/json"
            );
        }
    }
);

    const int port = 8081;

    std::cout << "Starting Atmosphere server on port "
              << port << "..." << std::endl;

    if (!server.bind_to_port("127.0.0.1", port)) {
        std::cerr << "Port " << port
                  << " is unavailable." << std::endl;
        return 1;
    }

    std::cout << "Atmosphere server running at "
              << "http://localhost:" << port << std::endl;

    server.listen_after_bind();

    return 0;
}


