const regionStates = {
  "PACIFIC NORTHWEST": [
    "Washington", "Oregon"
  ],
  "WEST COAST": [
    "California"
  ],
  "MOUNTAIN WEST": [
    "Idaho", "Montana", "Wyoming", "Colorado",
    "Utah", "Nevada"
  ],
  "SOUTHWEST": [
    "Arizona", "New Mexico"
  ],
  "MIDWEST": [
    "Illinois", "Indiana", "Iowa", "Kansas",
    "Michigan", "Minnesota", "Missouri",
    "Nebraska", "North Dakota", "Ohio",
    "South Dakota", "Wisconsin"
  ],
  "NORTHEAST": [
    "Connecticut", "Maine", "Massachusetts",
    "New Hampshire", "New Jersey", "New York",
    "Pennsylvania", "Rhode Island", "Vermont"
  ],
  "MID-ATLANTIC": [
    "Delaware", "Maryland", "Virginia",
    "West Virginia"
  ],
  "SOUTHEAST": [
    "Alabama", "Florida", "Georgia", "Kentucky",
    "Mississippi", "North Carolina",
    "South Carolina", "Tennessee"
  ],
  "SOUTH CENTRAL": [
    "Arkansas", "Louisiana", "Oklahoma", "Texas"
  ],
  "ALASKA": ["Alaska"],
  "HAWAII": ["Hawaii"],
  "CARIBBEAN": ["Puerto Rico"]
};

const stateAbbreviations = {
  "VA": "Virginia",
  "DE": "Delaware"
};

function getRegion(state) {
  const normalizedState = stateAbbreviations[state] || state;

  for (const [region, states] of Object.entries(regionStates)) {
    if (states.includes(normalizedState)) {
      return region;
    }
  }

  return "UNITED STATES";
}

function getSeason(month) {
  const seasons = {
    January: "WINTER",
    February: "WINTER",
    March: "SPRING",
    April: "SPRING",
    May: "SPRING",
    June: "SUMMER",
    July: "SUMMER",
    August: "SUMMER",
    September: "AUTUMN",
    October: "AUTUMN",
    November: "AUTUMN",
    December: "WINTER"
  };

  return seasons[month] || "SEASON";
}
