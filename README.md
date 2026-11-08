
# Atmosphere 

### Music Recommendation System Based on Weather and Popular Cities
**COP3530 — Data Structures & Algorithms | University of Florida**

**Team Name:** AlgoRhythm (Group 52)

**Team Members:**
- Kyra Paim — GitHub: [@kyrapaim](https://github.com/kyrapaim)
- Hannah Johns — GitHub: [@hannahpjohns](https://github.com/hannahpjohns)
- Yuxuan Tao — GitHub: [@yuxuantao12](https://github.com/yuxuantao12)

**Project Repository:** https://github.com/kyrapaim/Atmosphere

**Live Website:** https://kyrapaim.github.io/Atmosphere/

**Video Demonstration:** [Add video link]

---

## Project Overview

Atmosphere is a music recommendation application that generates personalized playlists based on the weather conditions associated with a user's selected city and month.

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

Users can choose between two ranking algorithms:

1. **Merge Sort**
2. **Heap Sort**

Both algorithms rank songs by compatibility score, allowing users to compare their results and execution times. 

---

## Live Application

**Frontend:** https://kyrapaim.github.io/Atmosphere/

**Backend API:** https://atmosphere-nxj4.onrender.com

**API Health Check:** https://atmosphere-nxj4.onrender.com/health

The application uses a distributed architecture:

1. The user interacts with the website hosted on GitHub Pages.
2. The frontend collects the user's city, month, playlist size, and algorithm selections.
3. JavaScript sends an HTTP request to the C++ backend hosted on Render.
4. The backend retrieves the appropriate weather profile.
5. Based on the average monthly weather of the chosen city, all of the 100k songs loaded in by the Python script receive target values based on weather analytics. These target values are compared against the song's music analytics and produce a compatibility score of how closely it matches the weather conditions.
6. The selected algorithm organizes scores of the 100k ranked songs from highest to lowest.
7. The backend returns the highest ranked songs as JSON.
8. The frontend displays the playlist and algorithm execution time.

**Note:** The Render free hosting plan may spin down after inactivity. The first request after an idle period can take approximately 50 seconds or longer while the server restarts. This is not a reflection of the algorithm but rather the hosting site itself.

---

## Features Implemented

### Recommendations

Users can select a city and month to generate a playlist based on historical monthly weather averages.

### Compatibility Scoring

Every song is assigned a numerical compatibility score based on its musical characteristics and the selected weather profile.

### Algorithm Selection

Users can select Merge Sort or Max Heap to rank the songs.

### Adjustable Playlist Size

Users can choose how many songs they want in their generated playlist. Anywhere from 1-1000 out of 100k songs.

### Algorithm Performance Comparison

The application measures the execution time of the selected ranking algorithm and displays the result in milliseconds.

### Public Deployment

The frontend is deployed through GitHub Pages, while the C++ backend is deployed through Render using Docker.

---

### Music Dataset

**Source:** Million Song Dataset

Website: http://millionsongdataset.com/

The application uses a processed CSV containing approximately 100,000 songs.

File location:

`data/music_100k.csv`

Attributes discussed above are stored using the `Song` structure defined in `include/Song.h`.

### Weather Dataset

**Source:** CORGIS Weather Dataset

Website: https://corgis-edu.github.io/corgis/csv/weather/

File location:

`data/weather.csv`

The weather data contains historical observations from 2016.

Weather reports are grouped by city, state, and month to calculate monthly averages.

These averages are represented by the `WeatherProfile` structure.

### Dataset Integration

The datasets do not share a common identifier.

Instead, Atmosphere connects them through a custom scoring function.

For a selected city and month, the backend retrieves the corresponding weather profile and calculates a compatibility score for every song.

The resulting collection of `ScoredSong` objects is then ranked using the selected algorithm.

---

## Algorithms Implemented

### Merge Sort

Merge Sort is a divide-and-conquer sorting algorithm.

It works by:

1. Dividing the song collection into smaller halves.
2. Recursively sorting each half.
3. Merging the sorted halves into one ranked collection.
4. Returning a playlist with requested number of songs that contains the highest ranking.

The implementation ranks songs in descending order of compatibility score.

For songs with identical scores, song IDs provide a consistent tie-breaking rule.

**Worst-case time complexity:** O(n log n)

**Auxiliary space complexity:** O(n)

Implementation:

- `include/MergeSort.h`
- `src/MergeSort.cpp`

### Heap Sort

Implemented using a Max Heap. A Max Heap is a complete binary tree represented using an array.

The highest-priority song is maintained at the root.

The implementation works by:

1. Inserting scored songs into the heap.
2. Using `heapifyUp()` to maintain heap order.
3. Removing the highest ranked song with `extractMax()`.
4. Using `heapifyDown()` to restore heap order.
5. Repeating extraction until the requested number of songs has been returned.

The heap uses the same ranking rules as Merge Sort to produce consistent recommendations.

**Worst-case insertion:** O(log n)

**Worst-case extraction:** O(log n)

**Building the heap using n repeated insertions:** O(n log n)

**Extracting k recommendations:** O(k log n)

**Overall ranking complexity:** O(n log n + k log n)

**Heap storage:** O(n)

Implementation:

- `include/MaxHeap.h`
- `src/MaxHeap.cpp`

### Shared Ranking Comparator

Both algorithms use a common ranking rule defined in:

`include/Ranking.h`

The comparator prioritizes:

1. Higher compatibility scores.
2. Smaller lexicographical song IDs when scores are equal.

Using a shared comparator ensures that the two algorithms produce the same ranking order when given identical inputs.

---

## Technologies and Libraries

### Programming Languages

- C++
- JavaScript
- HTML
- CSS
- CMake
- Dockerfile syntax
- YAML

### Tools

- CLion
- Visual Studio Code
- Git
- GitHub
- GitHub Actions
- CMake
- Ninja
- Docker
- Render
- GitHub Pages
- Figma

### Libraries

**C++ Standard Library**

Used for vectors, strings, hash maps, filesystem operations, sorting-related utilities, and execution timing.

**cpp-httplib**

A header-only C++ HTTP server library used to expose the recommendation API.

https://github.com/yhirose/cpp-httplib

**nlohmann/json**

A C++ JSON library used to parse requests and construct responses.

https://github.com/nlohmann/json

The HTTP and JSON library headers are included directly in the repository.

---

## Project Structure

```text
Atmosphere/
│
├── CMakeLists.txt
├── Dockerfile
├── README.md
│
├── data/
│   ├── music_100k.csv
│   └── weather.csv
│
├── include/
│   ├── Song.h
│   ├── Weather.h
│   ├── ScoredSong.h
│   ├── DataLoader.h
│   ├── Scoring.h
│   ├── Ranking.h
│   ├── MergeSort.h
│   ├── MaxHeap.h
│   ├── httplib.h
│   └── json.hpp
│
├── src/
│   ├── main.cpp
│   ├── server.cpp
│   ├── DataLoader.cpp
│   ├── Scoring.cpp
│   ├── MergeSort.cpp
│   └── MaxHeap.cpp
│
├── frontend/
│   ├── index.html
│   ├── results.html
│   ├── script.js
│   ├── results.js
│   ├── style.css
│   ├── results.css
│   ├── globals.css
│   ├── cities.js
│   └── regions.js
│
└── .github/
    └── workflows/
        └── deploy-pages.yml
```

---

## How to Run the Project

There are two ways to evaluate Atmosphere:

1. Use the deployed public website.
2. Download and run the C++ source code locally.

### 1. Running the Public Website

This is the easiest method and requires no installation.

1. Open https://kyrapaim.github.io/Atmosphere/
2. Select a city.
3. Select a month.
4. Select the number of songs.
5. Choose Merge Sort or Max Heap.
6. Generate the playlist.
7. Review the recommendations and algorithm execution time.

The website communicates with the publicly deployed C++ backend.

**Note:** The backend may need time to wake up after inactivity on Render's free hosting plan. First request may take a while to execute, after that, runtime should be similar to how it would be if it was run locally.

### 2. Running the C++ Project Locally

#### Prerequisites

Install:

- Git
- A C++20-compatible compiler
- CMake 3.25 or newer
- A supported build system such as Ninja or Make

#### Step 1: Clone the Repository

Open a terminal and run:

```bash
git clone https://github.com/kyrapaim/Atmosphere.git
cd Atmosphere
```

#### Step 2: Verify the Dataset Files

Confirm that the following files exist:

```text
data/music_100k.csv
data/weather.csv
```

Both files are required for the recommendation system.

#### Step 3: Configure the Project

Using CMake:

```bash
cmake -S . -B build
```

If CMake requires a generator, you can use Ninja:

```bash
cmake -S . -B build -G Ninja
```

#### Step 4: Compile the Project

```bash
cmake --build build
```

The project defines two executables:

- `Atmosphere` — console-based application for testing the recommendation algorithms.
- `AtmosphereServer` — HTTP backend used by the frontend.

#### Step 5: Run the Console Application

From the repository root:

On Linux/macOS:

```bash
./build/Atmosphere
```

On Windows PowerShell:

```powershell
.\build\Atmosphere.exe
```

Depending on the CMake generator and build configuration, the executable may instead be inside a configuration-specific directory such as `build/Debug/`.

The console application loads the datasets and runs its built-in recommendation tests.

#### Step 6: Run the HTTP Server

From the repository root:

On Linux/macOS:

```bash
./build/AtmosphereServer
```

On Windows PowerShell:

```powershell
.\build\AtmosphereServer.exe
```

The server loads the music and weather datasets and begins listening for HTTP requests.

For local development, the default port is 8081.

Open:

http://localhost:8081/health

Expected response:

```json
{
  "message": "Atmosphere backend is running",
  "status": "ok"
}
```

#### Step 7: Run the Frontend Locally

Open the `frontend/` directory using Visual Studio Code.

Use the Live Server extension to open `index.html`.

The local frontend must send API requests to the locally running backend:

```javascript
http://127.0.0.1:8081/recommend
```

The deployed frontend instead uses:

```javascript
https://atmosphere-nxj4.onrender.com/recommend
```

If testing the local backend, temporarily change the request URL in `frontend/results.js` to the local address.

---

## API Documentation

The backend exposes three endpoints.

### GET /health

Checks whether the backend is running.

Example:

```text
https://atmosphere-nxj4.onrender.com/health
```

Example response:

```json
{
  "message": "Atmosphere backend is running",
  "status": "ok"
}
```

### GET /data-status

Returns the number of loaded songs and monthly weather profiles.

Example:

```text
https://atmosphere-nxj4.onrender.com/data-status
```

Response fields:

- `songsLoaded`
- `weatherProfilesLoaded`

### POST /recommend

Generates a weather-based playlist.

Request body:

```json
{
  "city": "Gainesville, Florida",
  "month": "July",
  "count": 10,
  "algorithm": "merge"
}
```

Accepted algorithm values:

- `"merge"`
- `"heap"`

The backend validates the request, identifies the corresponding monthly weather profile, scores the songs, and runs the selected algorithm.

The response includes:

- Selected city and month
- Selected algorithm
- Requested number of songs
- Returned number of songs
- Algorithm execution time in milliseconds
- Weather profile
- Ranked song recommendations

Each recommendation includes:

- Song ID
- Song title
- Artist
- Compatibility score

Example response structure:

```json
{
  "city": "Gainesville, Florida",
  "month": "July",
  "algorithm": "merge",
  "requestedCount": 10,
  "returnedCount": 10,
  "executionTimeMs": 25.4,
  "weather": {
    "temperature": 83.0,
    "precipitation": 0.298,
    "windSpeed": 3.1
  },
  "songs": [
    {
      "id": "EXAMPLE_SONG_ID",
      "title": "Example Song",
      "artist": "Example Artist",
      "score": 90.2
    }
  ]
}
```


### Example API Request Using curl

```bash
curl -X POST https://atmosphere-nxj4.onrender.com/recommend \
  -H "Content-Type: application/json" \
  -d '{"city":"Gainesville, Florida","month":"July","count":10,"algorithm":"heap"}'
```

---

## How to Compare the Performance Between the Algorithms

To compare Merge Sort and Max Heap:

1. Select the same city.
2. Select the same month.
3. Request the same number of songs.
4. Generate a playlist using Merge Sort.
5. Record the execution time.
6. Repeat using Max Heap.
7. Compare the ranking results and execution times.

Because both algorithms use the same compatibility scores and tie-breaking rule, they should return the same ranked recommendations.

Execution times may differ because Merge Sort and Max Heap use different operations to rank the collection.

Timing results may also vary across computers, server loads, and repeated executions.

---

## Deployment Architecture

### Frontend — GitHub Pages

The frontend consists of static HTML, CSS, and JavaScript files.

GitHub Actions publishes the `frontend/` directory using:

`.github/workflows/deploy-pages.yml`


### Backend — Render

The C++ backend is hosted as a Docker-based Render web service.

The Dockerfile:

1. Creates a Linux build environment.
2. Installs the necessary build tools.
3. Compiles the C++ server.
4. Copies the required datasets.
5. Starts the server executable.

The backend reads Render's `PORT` environment variable and listens on the appropriate network interface.

### Frontend–Backend Communication

The frontend sends HTTP POST requests containing the user's selections.

The backend returns JSON with the weather profile, recommended songs, and algorithm execution time.

CORS headers allow the GitHub Pages frontend to communicate with the Render backend.

---

## Troubleshooting

### Website Loads but No Songs Appear

- Check whether the Render backend is awake.
- Open the `/health` endpoint.
- Verify that `frontend/results.js` uses the correct backend URL.
- Check the browser's developer console for request errors.

### Backend Cannot Find CSV Files

Run the executable from the repository root.

Verify that both required CSV files are inside the `data/` directory.

### Port 8081 Is Already in Use

Stop any existing instance of `AtmosphereServer` before launching another one.

### Render Deployment Fails

Review the Render deployment logs.

Check that the Docker image builds successfully, the executable starts, and the runtime has the required C++ standard library dependencies.

### First Request Takes a Long Time

Render's free instance may have spun down due to inactivity. Wait for the service to restart and retry if necessary.

---

## References

1. Million Song Dataset — http://millionsongdataset.com/
2. CORGIS Weather Dataset — https://corgis-edu.github.io/corgis/csv/weather/
3. cpp-httplib — https://github.com/yhirose/cpp-httplib
4. nlohmann/json — https://github.com/nlohmann/json
5. CMake Documentation — https://cmake.org/documentation/
6. GitHub Pages Documentation — https://docs.github.com/en/pages
7. Render Documentation — https://render.com/docs

---
