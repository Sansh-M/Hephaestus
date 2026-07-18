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
	static float atmos_demsity_now(int planetID, std::string timeOfDay, std::string planetaryAngle, int altitude, float temperature);

	static float lookup_atmos_pressure(int planetID, std::string timeOfDay, int altitude);

	static float drag(float atmos_density, float velocity, int entityID, int planetID, std::string timeOfDay, int altitude, float temperature);

	static float calc_C_D(int entityID);
};

