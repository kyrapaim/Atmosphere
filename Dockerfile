# Stage 1: Compile the C++ backend
FROM gcc:14-bookworm AS builder

# Install CMake and Ninja to build the project
RUN apt-get update && \
    apt-get install -y cmake ninja-build && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the source code and CMake configuration
COPY CMakeLists.txt .
COPY include/ include/
COPY src/ src/

# Compile AtmosphereServer in Release mode
RUN cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --target AtmosphereServer

# Stage 2: Create the image that runs on Render
FROM debian:bookworm-slim

WORKDIR /app

# Copy the compiled C++ server
COPY --from=builder /app/build/AtmosphereServer /app/AtmosphereServer

# Copy the datasets used by the recommendation system
COPY data/music_100k.csv data/music_100k.csv
COPY data/weather.csv data/weather.csv

# Document the default port used by Render
EXPOSE 10000

# Start the actual C++ server
CMD ["./AtmosphereServer"]
