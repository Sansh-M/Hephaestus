#pragma once
#include <iostream>
#include <fstream>
#include "PlanetaryBody.hpp"
#include <vector>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

/*
Class for functions used to initialize the physics engine when first opened.
Class constructs all the objects from the json files to initialize the universe.
*/
class Universe_init {

public:
	/*
	Builds a vector containing all of the planetary bodies in the universe.
	*/
	std::vector<PlanetaryBody> buildPlanetaryBodies(const std::string& json_path);

private:
	/*
	Build a planetary body object using the json data
	*/
	PlanetaryBody BuildPlanetaryBody(const json& p);
};



