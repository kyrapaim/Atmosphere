document.addEventListener("DOMContentLoaded", async () => {
    const saved = sessionStorage.getItem("atmosphereSelections");

    const locationLabel = document.getElementById("location-label");
    const description = document.getElementById("playlist-description");
    const algorithmLabel = document.getElementById("algorithm-label");
    const songTotal = document.getElementById("song-total");
    const container = document.getElementById("playlist-results");

    // Make sure the user generated a playlist first
    if (!saved) {
        description.textContent =
            "No playlist found. Return to the builder to create one.";
        return;
    }

    let selections;

    try {
        selections = JSON.parse(saved);
    } catch (error) {
        description.textContent = "Invalid playlist selections.";
        console.error(error);
        return;
    }

    // Display the user's selections
    locationLabel.textContent =
        `${selections.city} · ${selections.month}`;

    description.textContent =
        `${selections.count} songs requested for your atmosphere`;

    algorithmLabel.textContent =
        selections.algorithm === "merge"
            ? "Merge Sort selected"
            : "Heap Sort selected";

    songTotal.textContent = "Generating playlist...";

    try {
        // Send selections to your C++ backend
        const response = await fetch ("https://atmosphere-nxj4.onrender.com/recommend", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify(selections)
            }
        );

        const data = await response.json();

        if (!response.ok) {
            throw new Error(
                data.error || "Recommendation request failed"
            );
        }

        console.log("Backend recommendations:", data);

        // Display actual number of songs and execution time
        description.textContent =
            `${data.returnedCount} songs generated for your atmosphere`;

        songTotal.textContent =
            `${data.returnedCount} songs · ` +
            `${data.executionTimeMs.toFixed(2)} ms`;

        // Remove any previous results
        container.replaceChildren();

        // Display real C++ recommendations
        data.songs.forEach((song, index) => {
            const row = document.createElement("div");
            row.className = "song-row";

            const number = document.createElement("span");
            number.textContent = index + 1;

            const info = document.createElement("div");
            info.className = "song-info";

            const title = document.createElement("strong");
            title.textContent = song.title;

            const artist = document.createElement("small");
            artist.textContent = song.artist;

            info.append(title, artist);

            const score = document.createElement("span");
            score.className = "match-score";
            score.textContent = `${song.score.toFixed(1)}%`;

            row.append(number, info, score);
            container.appendChild(row);
        });

    } catch (error) {
        console.error("Atmosphere backend error:", error);

        songTotal.textContent = "Unable to generate playlist";

        description.textContent =
            `Error: ${error.message}`;
    }
});
