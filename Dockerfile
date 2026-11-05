
# Use GCC 14 to compile and run the C++ application.
FROM gcc:14-bookworm

# Install CMake and Ninja.
RUN apt-get update && \
    apt-get install -y cmake ninja-build && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the project files.
COPY CMakeLists.txt .
COPY include/ include/
COPY src/ src/

# Copy the datasets.
COPY data/music_100k.csv data/music_100k.csv
COPY data/weather.csv data/weather.csv

# Compile the C++ backend.
RUN cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --target AtmosphereServer

# Render provides the port through an environment variable.
EXPOSE 10000

# Start the server.
CMD ["./build/AtmosphereServer"]
