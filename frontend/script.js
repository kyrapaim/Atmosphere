
const monthButtons = document.querySelectorAll(
    ".month-option, .month-name-wrapper"
);

const countButtons = document.querySelectorAll(
    ".song-count-option, .song-count-wrapper"
);

let selectedMonth = "October";
let selectedCount = 25;
let selectedCity = "Seattle, Washington";

// Set the initial selections
document.querySelector(".month-name-wrapper")
    ?.classList.add("active-choice");

document.querySelector(".song-count-wrapper")
    ?.classList.add("active-choice");

// MONTH SELECTION
monthButtons.forEach(button => {
    button.addEventListener("click", () => {
        selectedMonth = button.textContent.trim();

        monthButtons.forEach(item => {
            item.classList.remove("active-choice");
        });

        button.classList.add("active-choice");

        updatePreview();
        console.log("Selected month:", selectedMonth);
    });
});

// SONG COUNT SELECTION
countButtons.forEach(button => {
    button.addEventListener("click", () => {
        selectedCount = Number(button.textContent.trim());

        countButtons.forEach(item => {
            item.classList.remove("active-choice");
        });

        button.classList.add("active-choice");

        console.log("Selected count:", selectedCount);
    });
});

// UPDATE PREVIEW

function updatePreview() {
    const title = document.querySelector(".mood-title");
    const regionSeasonLabel = document.getElementById(
        "preview-region-season"
    );

    // Split at the final comma so city names are preserved.
    const separator = selectedCity.lastIndexOf(",");

    if (separator === -1) return;

    const city = selectedCity.slice(0, separator).trim();
    const state = selectedCity.slice(separator + 1).trim();

    // Update city/month heading
   if (title) {
    title.replaceChildren(
        document.createTextNode(`${city} in`),
        document.createElement("br"),
        document.createTextNode(`${selectedMonth}.`)
    );
}

    // Update region/season label
    if (regionSeasonLabel) {
        const region = getRegion(state);
        const season = getSeason(selectedMonth);

        regionSeasonLabel.textContent = `${region} · ${season}`;
    }
}


// GENERATE PLAYLIST
const generateButton = document.querySelector(".generate-playlist");

if (generateButton) {
    generateButton.addEventListener("click", () => {
    const selections = {
        city: selectedCity,
        month: selectedMonth,
        count: selectedCount,
        algorithm: selectedAlgorithm
    };

    sessionStorage.setItem(
        "atmosphereSelections",
        JSON.stringify(selections)
    );

    window.location.href = "results.html";
    });
}


const cityInput = document.getElementById("city-input");
const cityResults = document.getElementById("city-results");
const cityName = document.querySelector(".city-name");


cityInput.addEventListener("input", () => {
    const search = cityInput.value.trim().toLowerCase();

    cityResults.replaceChildren();

    if (!search) {
        cityResults.hidden = true;
        return;
    }

    const matches = cities.filter(city =>
        city.toLowerCase().includes(search)
    );

    matches.forEach(city => {
        const button = document.createElement("button");
        button.type = "button";
        button.className = "city-result";
        button.textContent = city;

        button.addEventListener("click", () => {
            selectedCity = city;
            cityName.textContent = city;
            cityInput.value = city;
            cityResults.hidden = true;

            updatePreview();
        });

        cityResults.appendChild(button);
    });

    cityResults.hidden = matches.length === 0;
});

let selectedAlgorithm = "merge";

document.querySelectorAll(".algorithm-option").forEach(button => {
    button.addEventListener("click", () => {
        selectedAlgorithm = button.dataset.algorithm;

        document.querySelectorAll(".algorithm-option").forEach(item => {
            item.classList.remove("active-choice");
        });

        button.classList.add("active-choice");
    });
});

updatePreview();
