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

struct AtmosphereRow {
	double altitudeKm;   // Altitude column, measured in kilometres.
	double pressureBar;  // Pressure column, measured in bar.
	double temperatureK; // Temperature column, measured in kelvin.
};

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
		row.pressureBar = std::stod(pressure);    // stod also accepts scientific notation.
		row.temperatureK = std::stod(temperature);
	}
	catch (const std::exception&) {
		return false; // At least one field was not a valid number.
	}

	return true; // The complete CSV row was parsed successfully.
}


static float atmos_demsity_now(PlanetaryBody& planet, std::string planetaryAngle, std::string altitude, float temperature) {
	std::string atmos_pressure_path = build_csv_path(planet, timeOfDay, altitude);

	float atmos_pressure_now = 

	//lookup molar mass using planetID
	float molarmass=0.0; //lookup molarmass
	float atmos_density = (atmos_pressure_now * molarmass) / (Physics::UNIVERSAL_GAS_CONSTANT * temperature);
	return atmos_density;
}

	
std::string build_csv_path(PlanetaryBody& planet, std::string timeOfDay, std::string altitude) {
	std::string lookup_file_name = std::to_string(0);
	lookup_file_name.append(planet.getName()); //appends name and converts it to fully uppercase
	lookup_file_name.append(std::to_string(1));
	lookup_file_name.append(timeOfDay);
	lookup_file_name.append(std::to_string(2));
	lookup_file_name.append(timeOfDay);
	std::string path = "src/Data/Atmosphere_Data/";
	path.append(lookup_file_name).append(".csv");
}

std::optional<double> lookup_atmos_pressure(const std::string& atmos_pressure_path, double altitude) { // Returns a pressure or no value.
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

			const double maximumAltitude =
				std::max(previous.altitudeKm, current.altitudeKm);

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

}

static float calc_C_D(int entityID) {

}

	
