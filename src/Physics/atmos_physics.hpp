#include <iostream>
#include <string>
#include "../constants.hpp"
#include "PlanetaryBody.hpp"
#include <fstream>
#include <vector>
#include <sstream>
#include <optional>

class atmos_physics {
public:

	static bool parseAtmosphereRow(const std::string& line, AtmosphereRow& row);

	static float atmos_demsity_now(PlanetaryBody& planet, std::string planetaryAngle, double altitude, float temperature);

	static std::string build_csv_path(const PlanetaryBody& planet);

	static std::optional<double> lookup_atmos_pressure(const std::string& atmos_pressure_path, double altitude)

	static float drag(float atmos_density, float velocity, int entityID, int planetID, std::string timeOfDay, int altitude, float temperature);

	static float calc_C_D(int entityID);
};

