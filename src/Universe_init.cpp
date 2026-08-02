#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "PlanetaryBody.hpp"
#include "constants.hpp"
#include "Universe_init.hpp"
#include "PlanetaryRotation.hpp"

using json = nlohmann::json;
const std::string JSONPATH = "src/Data/Universe.json";

/*
Builds a vector containing all of the planetary bodies in the universe. 
*/
std::vector<PlanetaryBody> Universe_init::buildPlanetaryBodies(const std::string& json_path) {
	std::ifstream file(json_path);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open " + json_path);

	}

	json data = json::parse(file, nullptr, true, true);	//initialize json object called data with all the contents of Universe.json
	std::vector <PlanetaryBody> planetary_bodies; //create array of planetary bodies object
	planetary_bodies.reserve(data.at("planets").size());	//allocate memory for the number of planets in the json file

	for (const auto& p : data.at("planets")) {
		planetary_bodies.push_back(Universe_init::BuildPlanetaryBody(p)); //add plaent to the back of the planetary_bodies vector. 
	}

	return planetary_bodies;
}

/*
Build a planetary body object using the json data
*/
PlanetaryBody Universe_init::BuildPlanetaryBody(const json& p) {
	std::string name = p.at("name").get<std::string>();

	Vec3 origin{
		p.at("origin").at("x").get<float>(),
		p.at("origin").at("y").get<float>(),
		p.at("origin").at("z").get<float>()
	};

	Vec3 pos = origin;

	float mass = p.at("mass").get<float>();
	float radius = p.at("radius").get<float>();

	std::vector<std::string> CSVPATHS;
	if (p.contains("CSVPATHS")) {
		const auto& paths = p.at("CSVPATHS");
		for (size_t i = 0; i < paths.size(); ++i) {
			CSVPATHS.push_back(paths.at(i).get<std::string>());
		}
	}

	OrbitParams orbit_params = { 0, 0, 0, 0, 0, 0, 0 }; // Default values for Sun
	RotationFrame rotation_frame = { 1, 0, 0, 1, 0, 0 }; // Identity matrix for Sun
	std::string parent = "";

	if (name != "Sun") {
		parent = p.at("parent");
		float semiMajorAxis = p.at("semiMajorAxis").get<float>();
		float eccentricity = p.at("eccentricity").get<float>();
		float orbitalPeriod = p.at("orbitalPeriod").get<float>();
		float meanAnomaly0 = p.at("meanAnomaly0").get<float>();
		float inclination = p.at("inclination").get<float>();
		float raan = p.at("raan").get<float>();
		float argOfPeriapsis = p.at("argOfPeriapsis").get<float>();
		orbit_params = { semiMajorAxis, eccentricity, orbitalPeriod, meanAnomaly0, inclination, raan, argOfPeriapsis };
	
		float R11 = (cos(raan) * cos(argOfPeriapsis))
			- sin(raan) * sin(argOfPeriapsis) * cos(	inclination); // R11 computation 
		float R12 = -(cos(raan) * sin(argOfPeriapsis)) - sin(raan) * cos(argOfPeriapsis) * cos(inclination);
		float R21 = sin(raan) * cos(argOfPeriapsis) + cos(raan) * sin(argOfPeriapsis) * cos(inclination);
		float R22 = -(sin(raan) * sin(argOfPeriapsis)) + cos(raan) * cos(argOfPeriapsis) * cos(inclination);
		float R31 = sin(argOfPeriapsis) * sin(inclination);
		float R32 = cos(argOfPeriapsis) * sin(inclination);

		rotation_frame = { R11, R12, R21, R22, R31, R32 };
	
	}
	

	return PlanetaryBody(name, origin, pos, 0.0, mass, radius, CSVPATHS, parent, orbit_params, rotation_frame);
};

