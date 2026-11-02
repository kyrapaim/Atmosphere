
const saved = sessionStorage.getItem("atmosphereSelections");

const selections = saved ? JSON.parse(saved) : {
    city: "Seattle, Washington",
    month: "October",
    count: 25,
    algorithm: "merge"
};

// Sample data ONLY — not scored by C++
const mockSongs = [
    { title: "Sample Song A", artist: "Artist A", score: 94.8 },
    { title: "Sample Song B", artist: "Artist B", score: 93.7 },
    { title: "Sample Song C", artist: "Artist C", score: 92.9 },
    { title: "Sample Song D", artist: "Artist D", score: 91.6 },
    { title: "Sample Song E", artist: "Artist E", score: 90.8 }
];

document.getElementById("location-label").textContent =
    `${selections.city} · ${selections.month}`;

document.getElementById("playlist-description").textContent =
    `${selections.count} songs requested for your atmosphere`;

document.getElementById("algorithm-label").textContent =
    selections.algorithm === "merge"
        ? "Merge Sort selected"
        : "Max Heap selected";

document.getElementById("song-total").textContent =
    `${selections.count} songs requested`;

const container = document.getElementById("playlist-results");

// Repeat the sample records to demonstrate the selected length.
// This does not perform real ranking.
for (let i = 0; i < selections.count; i++) {
    const song = mockSongs[i % mockSongs.length];

    const row = document.createElement("div");
    row.className = "song-row";

    const number = document.createElement("span");
    number.textContent = i + 1;

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
}
