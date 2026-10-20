import csv
import math
from pathlib import Path
import tables

# Locate the project directory
PROJECT_DIR = Path(__file__).resolve().parent.parent

INPUT_FILE = PROJECT_DIR / "data" / "msd_summary_file.h5"
OUTPUT_FILE = PROJECT_DIR / "data" / "music_100k.csv"

TARGET = 100000


def decode(value):
    """Convert HDF5 byte strings into normal Python strings."""
    if isinstance(value, bytes):
        return value.decode("utf-8", errors="replace")
    return str(value)


def main():
    if not INPUT_FILE.exists():
        print(f"File not found: {INPUT_FILE}")
        return

    count = 0
    seen_ids = set()

    with tables.open_file(INPUT_FILE, mode="r") as h5:
        analysis = h5.root.analysis.songs
        metadata = h5.root.metadata.songs

        print(f"Available tracks: {analysis.nrows}")

        with open(
                OUTPUT_FILE, "w", newline="", encoding="utf-8"
        ) as output:

            writer = csv.writer(output)

            writer.writerow([
                "id",
                "title",
                "artist",
                "key",
                "tempo",
                "loudness"
            ])

            for index in range(analysis.nrows):
                a = analysis[index]
                m = metadata[index]

                song_id = decode(m["song_id"])
                title = decode(m["title"])
                artist = decode(m["artist_name"])

                key = float(a["key"])
                tempo = float(a["tempo"])
                loudness = float(a["loudness"])

                # Skip missing or invalid records
                if not song_id or song_id in seen_ids:
                    continue

                if not title.strip() or not artist.strip():
                    continue

                if not all(math.isfinite(x) for x in
                           [key, tempo, loudness]):
                    continue

                if tempo <= 0:
                    continue

                seen_ids.add(song_id)

                writer.writerow([
                    song_id,
                    title,
                    artist,
                    key,
                    tempo,
                    loudness
                ])

                count += 1

                if count % 10000 == 0:
                    print(f"Extracted {count} songs...")

                if count == TARGET:
                    break

    print(f"\nFinished! Extracted {count} songs.")
    print(f"Saved to: {OUTPUT_FILE}")

    if count < TARGET:
        print("Warning: Fewer than 100,000 valid songs found.")


if __name__ == "__main__":
    main()
