#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
const std::string JSONPATH = "src/Data/Universe.json";

/*
Class for functions used to initialize the physics engine when first opened. 
Class constructs all the objects from the json files to initialize the universe. 
*/
class Universe_init {

public: 
	/*
	Builds a vector containing all of the planetary bodies in the universe. 
	*/
	std::vector<PlanetaryBody> buildPlanetaryBodies(const std::string& json_path) {
		std::ifstream file(json_path);
		if (!file.is_open()) {
			throw std::runtime_error("Could not open " + json_path);

		}

		json data = json::parse(file, nullptr, true, true);	//initialize json object called data with all the contents of Universe.json
		std::vector <PlanetaryBody> planetary_bodies; //create array of planetary bodies object
		planetary_bodies.reserve(data.at("planets").size());	//allocate memory for the number of planets in the json file

		for (const auto& p : data.at("planets")) {
			planetary_bodies.push_back(BuildPlanetaryBody(p)); //add plaent to the back of the planetary_bodies vector. 
		}

		return planetary_bodies;
	}

private:
	/*
	Build a planetary body object using the json data
	*/
	PlanetaryBody BuildPlanetaryBody(const json& p) {
		std::string name = p.at("name").get<std::string>();

		vec3 origin{
			p.at("origin").at("x").get<float>(),
			p.at("origin").at("y").get<float>(),
			p.at("origin").at("z").get<float>()
		};

		float mass = p.at("mass").get<float>();
		float radius = p.at("radius").get<float>();

		std::vector<string> CSVPATHS;
		if (p.contains("CSVPATHS")) {
			const auto& paths = p.at("CSVPATHS");
			for (size_t i = 0; i < paths.size(); ++i) {
				CSVPATHS.push_back(paths.at(i).get<std::string>());
			}
		}

		float semiMajorAxis = p.at("semiMajorAxis").get<float>();
		float eccentricity = p.at("eccentricity").get<float>();
		float orbitalPeriod = p.at("orbitalPeriod").get<float>();
		float meanAnomaly0 = p.at("meanAnomaly0").get<float>();
		float inclination = p.at("inclination").get<float>();
		float raan = p.at("raan").get<float>();
		float argOfPeriapsis = p.at("argOfPeriapsis").get<float>();
		float orbitalPeriod = p.at("orbitalPeriod").get<float>();
		OrbitParams orbit_params = { semiMajorAxis, eccentricity, orbitalPeriod, meanAnomaly0, inclination, raan, argOfPeriapsis, orbitalPeriod };

		float R11 = (cos(planet.orbit_params.raan) * cos(planet.orbit_params.argOfPeriapsis))
			- sin(planet.orbit_params.raan) * sin(planet.orbit_params.argOfPeriapsis) * cos(planet.orbit_params.inclination); // R11 computation 
		float R12 = -(cos(raan) * sin(argOfPeriapsis)) - sin(raan) * cos(argOfPeriapsis) * cos(inclination);
		float R21 = sin(raan) * cos(argOfPeriapsis) + cos(raan) * sin(argOfPeriapsis) * cos(inclination);
		float R22 = -(sin(raan) * sin(argOfPeriapsis)) + cos(raan) * cos(argOfPeriapsis) * cos(inclination);
		float R31 = sin(argOfPeriapsis) * sin(inclination);
		float R32 = cos(argOfPeriapsis) * sin(inclination);

		RotationFrame rotation_frame = { R11, R12, R21, R22, R31, R32 };

		float angle0 = p.at("angle0").get<float>();
		float angularVelocity = p.at("angularVelocity").get<float>();

		RotationState rotation_state = { angle0, angularVelocity };

	}
		return PlanetaryBody(name, origin, mass, radius, CSVPATHS, orbit_paramas, rotation_frame, rotation_state);
	}
};



