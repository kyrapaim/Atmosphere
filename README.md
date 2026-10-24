Atmosphere uses two datasets:

Music Dataset — Million Song Dataset

- Source: [Million Song Dataset](http://millionsongdataset.com/)
- 100,000 songs extracted from the original HDF5 dataset using a Python script.
- Each song contains its ID, title, artist, musical key, mode, tempo, and loudness.

Weather Dataset — CORGIS

- Source: [CORGIS Weather Dataset](https://corgis-edu.github.io/corgis/csv/weather/)
- Contains historical weather observations from 2016 in different cities and states.
- We use city, state, month, average temperature, precipitation, and wind speed.
- Individual weather observations are grouped into monthly averages for each city.

The goal is to allow users to select a city and month, calculate compatibility scores for all songs based on that location's weather, and return the highest-scoring recommendations. 

## Project Structure

```text
Atmosphere/
│
├── data/
│   ├── music_100k.csv
│   └── weather.csv
│
├── scripts/
│   └── extract_music.py
│
├── include/
│   ├── Song.h
│   ├── Weather.h
│   ├── DataLoader.h
│   ├── Scoring.h
│   ├── ScoredSong.h
│   ├── MergeSort.h
│   └── MaxHeap.h
│
├── src/
│   ├── main.cpp
│   ├── DataLoader.cpp
│   ├── Scoring.cpp
│   ├── MergeSort.cpp
│   └── MaxHeap.cpp
│
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Important Files

| File | Purpose |
|---|---|
| `Song.h` | Defines the `Song` struct containing musical attributes |
| `Weather.h` | Defines `WeatherProfile` and declares weather-related functions |
| `ScoredSong.h` | Defines a struct containing a `Song` and its compatibility score |
| `DataLoader.h/.cpp` | Parses CSV files and loads music/weather data |
| `Scoring.h/.cpp` | Calculates weather-to-song compatibility scores |
| `MergeSort.h/.cpp` | Merge Sort implementation (to be completed) |
| `MaxHeap.h/.cpp` | Max Heap implementation (to be completed) |
| `main.cpp` | Connects data loading, weather selection, scoring, and eventually both algorithms |
| `extract_music.py` | Extracts 100,000 songs from the original HDF5 dataset into a CSV |

## How the Data Is Loaded

### Music Data

The original Million Song Dataset is stored in HDF5 format, which we preprocess using Python.

The `extract_music.py` script reads the original dataset and creates `music_100k.csv` containing:

```text
id,title,artist,key,mode,tempo,loudness
```

The Python script is only used for data extraction. The main application and algorithm implementations are written in C++.

`loadSongs()` in `DataLoader.cpp` reads the CSV and creates a `vector<Song>`.

The CSV parser handles commas inside quoted fields, such as song titles containing commas.

### Weather Data

`loadWeather()` reads `weather.csv` and creates a `vector<WeatherProfile>` containing individual weather observations.

Since we want to recommend music based on typical monthly conditions rather than specific days, these observations are passed into:

```cpp
calculateMonthlyAverages(weatherProfiles);
```

This function uses an `unordered_map` to group observations by city, state, and month.

Each map entry accumulates temperature, precipitation, and wind speed, while counting how many observations belong to that group.

After processing all observations, the function divides each accumulated measurement by the observation count and returns a vector containing one average `WeatherProfile` per city/state/month combination.

Note: These are historical averages grouped by calendar month, not live weather reports.

## Weather Selection

Two helper functions are already implemented:

**`getAvailableCities()`**

Returns an alphabetically sorted vector of unique city/state combinations from the weather dataset.

This will allow us to display valid city options instead of asking users to type a location that might not exist in our dataset.

**`findWeatherProfile()`**

Searches the monthly weather averages for a matching city, state, and month.

It returns a pointer to the matching `WeatherProfile`, or `nullptr` if no match exists.

Months are currently represented as numeric strings (`"1"` through `"12"`).

The full interactive city/month selection interface is not implemented yet, but the supporting functions are available.

## Song Compatibility Scoring

The scoring system is implemented in `Scoring.cpp`.

Every song receives a compatibility score from **0 to 100** based on the selected weather profile.

We use three musical attributes:

| Musical attribute | Weight |
|---|---:|
| Tempo (BPM) | 45% |
| Loudness (dB) | 35% |
| Mode (major/minor) | 20% |

### Weather Inputs

Currently, scoring uses:

- Temperature
- Precipitation

Wind speed remains in `WeatherProfile` but is excluded from scoring.

Musical key is also excluded because the pitch class alone does not reliably indicate musical mood.

### How Scoring Works

**Step 1 — Normalize Weather**

Temperature and precipitation are converted to values between 0 and 1:

```text
T = clamp((temperature - 32) / 68, 0, 1)
R = clamp(precipitation / 1.0, 0, 1)
```

**Step 2 — Calculate Musical Targets**

Based on the normalized weather, we calculate target tempo and loudness:

```text
Target BPM = 75 + 65(0.65T + 0.35(1-R))

Target Loudness = -16 + 12(0.70T + 0.30(1-R))
```

Warmer and drier conditions generally produce higher target tempo and loudness values.

**Step 3 — Calculate Compatibility**

Each song's actual tempo and loudness are compared against the calculated targets.

```text
Tempo Score = clamp(1 - |Song BPM - Target BPM| / 80, 0, 1)

Loudness Score = clamp(1 - |Song Loudness - Target Loudness| / 20, 0, 1)
```

Musical mode is scored using a weather-based preference for major versus minor mode:

```text
Major Preference = clamp(0.20 + 0.50T - 0.30R, 0.10, 0.90)
```

For a major-mode song, the mode score equals the major preference. For a minor-mode song, it equals `1 - Major Preference`.

**Step 4 — Final Score**

```text
Final Score = 100 × (
    0.45 × Tempo Score +
    0.35 × Loudness Score +
    0.20 × Mode Score
)
```

The scoring model is a heuristic we designed for this project. The weights and thresholds are initial assumptions, not scientifically validated relationships.

### Scoring Function

```cpp
double calculateScore(
    const Song& song,
    const WeatherProfile& weather
);
```

This function returns a `double` between 0 and 100.

## 6. The ScoredSong Struct — IMPORTANT FOR ALGORITHMS

The `ScoredSong` struct is defined in `ScoredSong.h`:

```cpp
struct ScoredSong {
    Song song;
    double score = 0.0;
};
```

It contains:

- `song`: The original song object, including title, artist, tempo, etc.
- `score`: The song's compatibility score for the selected weather.

The score is not stored in the music CSV because it changes depending on which city and month the user selects.

Instead, scores are calculated when the C++ application runs.

In `main.cpp`, we iterate through the loaded songs and calculate their scores:

```cpp
vector<ScoredSong> scoredSongs;

for (const Song& song : songs) {
    double score = calculateScore(song, *selectedWeather);
    scoredSongs.push_back({song, score});
}
```

After this loop, `scoredSongs` contains every loaded song and its corresponding compatibility score.

**Both Merge Sort and Max Heap should operate on `ScoredSong` objects. Neither algorithm needs to calculate compatibility

## Running the Project in CLion

1. Clone the Atmosphere GitHub repository.
2. Open the project folder in CLion.
3. Allow CLion to load the `CMakeLists.txt` configuration.
4. Confirm that `data/music_100k.csv` and `data/weather.csv` are available.
5. Build and run the `Atmosphere` executable.

If the generated music CSV is not included in the repository, it must be supplied or regenerated using `scripts/extract_music.py` and the original Million Song Dataset HDF5 file.

The Python extraction step requires PyTables:

```bash
pip install tables
```

To regenerate the music CSV from the project root:

```bash
python scripts/extract_music.py
```

**Important:** The C++ application reads CSV files using relative paths. Make sure CLion's run configuration uses the appropriate working directory. Depending on the configuration, you may need to adjust file paths.

The HDF5 file is not required to run the C++ application if the generated music CSV is already available.


We'll compare two scenarios:

**Full ranking:** Both algorithms produce a complete ranking of all songs. Merge Sort sorts the entire vector, while Max Heap extracts all songs.

**Top-10 recommendations:** Compare the work required to retrieve only the 10 highest-ranked songs.

For fair benchmarking, we'll time the algorithm operations separately from CSV loading, weather processing, scoring, and input copying.

We'll also verify that both algorithms return equivalent results, including how they handle tied scores.


### In Progress / To Do

- Implement Merge Sort
- Implement Max Heap
- Integrate both algorithms into `main.cpp`
- Generate top-10 recommendations
- Benchmark and compare both algorithms
- Implement interactive city/month selection
- Test additional cities, months, and edge cases
- Finalize project presentation and analysis

