#include <iostream>
#include <string>
#include "../constants.hpp"
#include "PlanetaryBody.hpp"
#include <fstream>
#include <vector>
#include <sstream>
#include <optional>
#include <algorithm>
#include <cmath>
#include <exception>
#include <cctype>
#include <map>

namespace {
	constexpr double BAR_TO_PASCALS = 100000.0;

	// Mean atmospheric molar masses in kg/mol. This is the unit required by
	// rho = pM/(RT) when pressure is in pascals and R is in J/(mol*K).
	const std::map<std::string, double> molarMassesKgPerMol = {
		{"Venus", 0.04345},
		{"Earth", 0.02897},
		{"Mars", 0.04334},
		{"Jupiter", 0.00222},
		{"Saturn", 0.00207},
		{"Titan", 0.02860},
		{"Uranus", 0.00264},
		{"Neptune", 0.00253}
	};
}

struct AtmosphereRow {
	double altitudeKm;   // Altitude column, measured in kilometres.
	double pressureBar;  // Pressure column, measured in bar.
	double temperatureK; // Temperature column, measured in kelvin.
};

static std::string build_csv_path(const PlanetaryBody& planet);
static std::optional<double> lookup_atmos_pressure(
	const std::string& atmos_pressure_path,
	double altitude);

static bool parseAtmosphereRow(const std::string& line, AtmosphereRow& row) {
	std::stringstream stream(line); // Lets us read one comma-separated field at a time.

	std::string altitude;
	std::string pressure;
	std::string temperature;

	if (!std::getline(stream, altitude, ',') || // Read text up to the first comma.
		!std::getline(stream, pressure, ',') ||
		!std::getline(stream, temperature, ',')) {
		return false; // The row did not contain all three required fields.
	}

	try {
		row.altitudeKm = std::stod(altitude);     // Convert string text to a double.
		row.pressureBar = std::stod(pressure);    // stod to accept scientific notation as well
		row.temperatureK = std::stod(temperature);
	}
	catch (const std::exception&) {
		return false; // At least one field was not a valid number.
	}

	return true; // The complete CSV row was parsed successfully.
}


static float atmos_demsity_now(PlanetaryBody& planet, std::string planetaryAngle, double altitude, float temperature) {

	std::string atmos_pressure_path = build_csv_path(planet);

	std::optional<double> atmos_pressure_now = lookup_atmos_pressure(atmos_pressure_path, altitude);
	if (!atmos_pressure_now) {
		return 0.0f;
	}

	const auto molarMass = molarMassesKgPerMol.find(planet.getName());
	if (molarMass == molarMassesKgPerMol.end() || temperature <= 0.0f) {
		return 0.0f;
	}

	const double pressurePa = *atmos_pressure_now * BAR_TO_PASCALS;
	const double atmosDensity =
		(pressurePa * molarMass->second) /
		(Physics::UNIVERSAL_GAS_CONSTANT * temperature);

	return static_cast<float>(atmosDensity);
}

	
static std::string build_csv_path(const PlanetaryBody& planet) {
	std::string name = planet.getName();
	std::transform(name.begin(), name.end(), name.begin(), [](unsigned char character) {
		return static_cast<char>(std::toupper(character));
	});

	std::string path = "src/Data/Atmosphere_Data/";
	return path.append(name).append(".csv");
}

static std::optional<double> lookup_atmos_pressure(const std::string& atmos_pressure_path, double altitude) { // Returns a pressure or no value.
	std::ifstream atmos_file(atmos_pressure_path); // Open the CSV for reading.

	if (!atmos_file.is_open()) {
		return std::nullopt; // optional contains no pressure value.
	}

	std::string line;

	// Skip the CSV header: Altitude,Pressure,TempKelvin
	std::getline(atmos_file, line); // Discard the header rather than parsing it as numbers.

	AtmosphereRow previous{}; // Previous valid CSV row, used as one interpolation endpoint.
	bool havePrevious = false; // The first row has no previous row to pair with.

	while (std::getline(atmos_file, line)) { // Read every remaining CSV row.
		AtmosphereRow current{}; // Parsed values from the row currently being examined.

		if (!parseAtmosphereRow(line, current)) {
			continue; // Ignore malformed or empty rows.
		}

		if (std::abs(current.altitudeKm - altitude) < 0.000001) {
			return current.pressureBar; // Requested altitude exactly matches this row.
		}

		if (havePrevious) {
			const double minimumAltitude = std::min(previous.altitudeKm, current.altitudeKm); // Works with either CSV order.

			const double maximumAltitude = std::max(previous.altitudeKm, current.altitudeKm);

			if (altitude >= minimumAltitude &&
				altitude <= maximumAltitude) { // The requested altitude lies between these rows.
				if (previous.pressureBar <= 0.0 || current.pressureBar <= 0.0) {
					return std::nullopt; // log() cannot be used with zero/negative pressure.
				}

				const double fraction =
					(altitude - previous.altitudeKm) /
					(current.altitudeKm - previous.altitudeKm); // 0 = previous row, 1 = current row.

				// Atmospheric pressure changes approximately
				// exponentially, so interpolate log(pressure).
				const double interpolatedLogPressure =
					std::log(previous.pressureBar) +
					fraction *
					(std::log(current.pressureBar) -
						std::log(previous.pressureBar));

				return std::exp(interpolatedLogPressure); // Convert log-pressure back to bar.
			}
		}

		previous = current; // Move the interpolation window down to the next pair.
		havePrevious = true;
	}

	// The requested altitude was outside the range covered by the CSV.
	return std::nullopt; // No two rows surrounded the requested altitude.
}


static float drag(float atmos_density, float velocity, int entityID, int planetID, std::string timeOfDay, int altitude, float temperature) {
	return 0.0f;
}

static float calc_C_D(int entityID) {
	return 0.0f;
}

	
