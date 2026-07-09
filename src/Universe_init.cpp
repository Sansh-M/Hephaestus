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
	std::vector<PlanetaryBody> buildPlanetaryBodies(const std::string& json_path) {
		//TODO: iterate through Universe.json and call buildPlanetaryBodies function. 
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
	PlanetaryBody BuildPlanetaryBody(const json& p) {
		std::string name = p.at("name").get<std::string>();

		vec3 origin{
			p.at("origin").at("x").get<float>(),
			p.at("origin").at("y").get<float>(),
			p.at("origin").at("z").get<float>()
		};

		float mass = p.at("mass").get<float>();
		float radius = p.at("radius").get<float>();

		std::string CSVPATHS[10];
		if (p.contains("CSVPATHS")) {
			const auto& paths = p.at("CSVPATHS");
			for (size_t i = 0; i < paths.size() && i < 10; ++i) {
				CSVPATHS[i] = paths.at(i).get<std::string>();
			}
		}

		return PlanetaryBody(name, origin, mass, radius, CSVPATHS);
	}
};



