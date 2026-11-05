FROM ubuntu:latest
LABEL authors="kyrap"

# Use a Linux environment with a C++ compiler.
FROM gcc:14-bookworm AS builder

# Install CMake and Ninja.
RUN apt-get update && \
    apt-get install -y cmake ninja-build && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the C++ source code and build configuration.
COPY CMakeLists.txt .
COPY include/ include/
COPY src/ src/

# Compile the server using the Release configuration.
RUN cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --target AtmosphereServer

# Create a smaller image for running the application.
FROM debian:bookworm-slim

WORKDIR /app

# Copy the compiled server.
COPY --from=builder /app/build/AtmosphereServer /app/AtmosphereServer

# Copy the datasets needed for recommendations.
COPY data/music_100k.csv data/music_100k.csv
COPY data/weather.csv data/weather.csv

# Render provides PORT at runtime.
EXPOSE 10000

# Start the C++ server.
CMD ["./AtmosphereServer"]

ENTRYPOINT ["top", "-b"]