#include <iostream>
#include "../constants.hpp"

class atmos_physics {
public: 
	static float atmos_demsity_now(int planetID, std::string timeOfDay, std::string planetaryAngle, int altitude, float temperature) {
		//TODO: open csv file. Use string comprehension to determine which path to use then return appropriate atmospheric density.
		float atmos_pressure_now = lookup_atmos_pressure(planetID, timeOfDay, altitude);
		//lookup molar mass using planetID
		float molarmass=0.0; //lookup molarmass
		float atmos_density = (atmos_pressure_now * molarmass) / (Physics::UNIVERSAL_GAS_CONSTANT * temperature);
	}
	
	static float lookup_atmos_pressure(int planetID, std::string timeOfDay, int altitude) {
		
	}

	static float drag(float atmos_density, float velocity, int entityID, int planetID, std::string timeOfDay, int altitude, float temperature) {

	}

	static float calc_C_D(int entityID) {

	}
};

	