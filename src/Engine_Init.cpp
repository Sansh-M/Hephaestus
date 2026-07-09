#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/*
Class for functions used to initialize the physics engine when first opened. 
Class constructs all the objects from the json files to initialize the universe. 
*/
class Engine_Init {

public: 
	void buildPlanetaryBodies(const json& Universe) {
		//TODO: iterate through Universe.json and call buildPlanetaryBodies function. 
	}


};